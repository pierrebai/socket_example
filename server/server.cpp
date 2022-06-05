#include <asio.hpp>

#include <string>
#include <iostream>

struct server_context
{
    // Parses command-line arguments and create a server_context, throws if missing arguments.
    static server_context parse_args(int argc, char* argv[]);

    int port_number = 0;
};


void serve_client(asio::ip::tcp::socket sock);

int main(int argc, char* argv[])
{
    try
    {
        auto server_context = server_context::parse_args(argc, argv);
        asio::io_context io_context;

        asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), server_context.port_number));
        while (true)
        {
            std::thread(serve_client, acceptor.accept()).detach();
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
            "    <port-number>: the port number to listen to."
        );

    server_context ctx;
    ctx.port_number = std::atoi(argv[1]);
    if (ctx.port_number <= 0 || ctx.port_number > 65535)
        throw std::exception("Invalid argument. The port number must be between 1 and 65535.");
    return ctx;
}

void serve_client(asio::ip::tcp::socket socket_to_client)
{
    try
    {
        while (true)
        {
            constexpr int max_length = 1024;
            char data[max_length];

            asio::error_code error;
            const size_t length = socket_to_client.read_some(asio::buffer(data), error);
            if (error == asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw asio::system_error(error); // Some other error.

            asio::write(socket_to_client, asio::buffer(data, length));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Serve client ended due to error: " << e.what() << "\n";
    }
}

