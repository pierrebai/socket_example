#include <asio.hpp>

#include <string>
#include <iostream>

struct client_context
{
    // Parses command-line arguments and create a client_context, throws if missing arguments.
    static client_context parse_args(int argc, char* argv[]);

    std::string server_name;
    std::string port_number;
};

int main(int argc, char* argv[])
{
    try
    {
        auto client_context = client_context::parse_args(argc, argv);
        asio::io_context io_context;

        auto server_endpoint = *asio::ip::tcp::resolver(io_context).resolve(client_context.server_name, client_context.port_number);
        asio::ip::tcp::socket socket_to_server(io_context);
        socket_to_server.connect(server_endpoint);

        while(true)
        {
            constexpr size_t max_length = 1024;
            char buffer[max_length];

            std::cin.getline(buffer, max_length);
            const size_t request_length = std::strlen(buffer);
            asio::write(socket_to_server, asio::buffer(buffer, request_length));

            const size_t reply_length = socket_to_server.read_some(asio::buffer(buffer, max_length));
            std::cout.write(buffer, reply_length);
            std::cout << "\n";
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
            "    <port-number>: the port number to connect to."
        );
    }

    client_context ctx;
    ctx.server_name = argv[1];
    ctx.port_number = argv[2];
    return ctx;
}

