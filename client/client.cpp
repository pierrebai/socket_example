#include <asio.hpp>

#include <string>
#include <iostream>

// Client configuration.
struct client_context
{
   // Parses command-line arguments and create a client_context, throws if missing arguments.
   static client_context parse_args(int argc, char* argv[]);

   std::string server_name;
   std::string port_number;
};

// Client connection to a server.
struct client
{
   // Creates the client that will use the given io context.
   client(asio::io_context& io_ctx);

   // Connect to the server specified in the client context.
   void connect(const client_context& client_ctx);

   // Read a line of text from standard input.
   std::string read_line() const;

   // Send text to the server.
   void send(const std::string& text);

   // Receive the reply of the server.
   std::string receive();

   asio::io_context& io_context;
   asio::ip::tcp::socket socket_to_server;
};

int main(int argc, char* argv[])
{
   try
   {
      auto client_context = client_context::parse_args(argc, argv);
      asio::io_context io_context;

      client client(io_context);
      client.connect(client_context);

      while (true)
      {
         std::string text = client.read_line();
         client.send(text);
         std::cout << client.receive() << std::endl;
      }
   }
   catch (std::exception& e)
   {
      std::cerr << "Exception: " << e.what() << "\n";
      return 1;
   }
}

client_context client_context::parse_args(int argc, char* argv[])
{
   if (argc < 3)
   {
      throw std::exception(
         "Missing arguments. Expected arguments:\n"
         "    <server-name>: the server to connect to.\n"
         "    <port-number>: the port number to connect to.");
   }

   client_context ctx;
   ctx.server_name = argv[1];
   ctx.port_number = argv[2];
   return ctx;
}

client::client(asio::io_context& io_ctx)
   : io_context(io_ctx), socket_to_server(io_context)
{
}

void client::connect(const client_context& client_ctx)
{
   auto server_endpoint = *asio::ip::tcp::resolver(io_context).resolve(client_ctx.server_name, client_ctx.port_number);
   socket_to_server.connect(server_endpoint);
}

std::string client::read_line() const
{
   constexpr size_t max_length = 1024;
   char buffer[max_length];
   std::cin.getline(buffer, max_length);
   return buffer;
}

void client::send(const std::string& text)
{
   asio::write(socket_to_server, asio::buffer(text.c_str(), text.size()));
}

std::string client::receive()
{
   constexpr size_t max_length = 1024;
   char buffer[max_length];
   const size_t reply_length = socket_to_server.read_some(asio::buffer(buffer, max_length));
   return std::string(buffer, reply_length);
}
