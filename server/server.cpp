#include <asio.hpp>

#include <string>
#include <optional>
#include <iostream>

// Server configuration.
struct server_context
{
   // Parses command-line arguments and create a server_context, throws if missing arguments.
   static server_context parse_args(int argc, char* argv[]);

   int port_number = 0;
};

// Server connection to a client.
struct server
{
   // Entry-point when a conneciton is made to a client to serve it.
   static void serve_client(asio::ip::tcp::socket sock);

   // Create a server to talk to a single client on a socket.
   server(asio::ip::tcp::socket&& socket);

   // Receive from the client.
   std::optional<std::string> receive();

   // Send to the client.
   void send(const std::string& text);

   asio::ip::tcp::socket socket_to_client;
};

int main(int argc, char* argv[])
{
   try
   {
      auto server_context = server_context::parse_args(argc, argv);
      asio::io_context io_context;

      asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), server_context.port_number));
      while (true)
      {
         #ifdef MULTI_THREADED_SERVER
            std::thread(server::serve_client, acceptor.accept()).detach();
         #else
            server::serve_client(acceptor.accept());
         #endif
      }
   }
   catch (std::exception& e)
   {
      std::cerr << "Exception: " << e.what() << "\n";
      return 1;
   }
}

server_context server_context::parse_args(int argc, char* argv[])
{
   if (argc < 2)
      throw std::exception(
         "Missing argument. Expected argument:\n"
         "    <port-number>: the port number to listen to.");

   server_context ctx;
   ctx.port_number = std::atoi(argv[1]);
   if (ctx.port_number <= 0 || ctx.port_number > 65535)
      throw std::exception("Invalid argument. The port number must be between 1 and 65535.");
   return ctx;
}

void server::serve_client(asio::ip::tcp::socket socket_to_client)
{
   try
   {
      server server(std::move(socket_to_client));

      while (true)
      {
         std::optional<std::string> data = server.receive();
         if (!data)
            break;
         server.send(data.value());
      }
   }
   catch (std::exception& e)
   {
      std::cerr << "Serve client ended due to error: " << e.what() << "\n";
   }
}

server::server(asio::ip::tcp::socket&& socket)
   : socket_to_client(std::move(socket))
{
}

std::optional<std::string> server::receive()
{
   constexpr int max_length = 1024;
   char data[max_length];

   asio::error_code error;
   const size_t length = socket_to_client.read_some(asio::buffer(data), error);

   // Verify if an error occured and handle end-of-file specially by returning
   // an empty result as it is the expected way to end the connection.
   if (error == asio::error::eof)
      return {};
   else if (error)
      throw asio::system_error(error);

   return std::string(data, length);

}
void server::send(const std::string& text)
{
   asio::write(socket_to_client, asio::buffer(text.c_str(), text.size()));
}
