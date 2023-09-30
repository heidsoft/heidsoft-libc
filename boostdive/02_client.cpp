#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

// 主函数
// g++ -o 02_client 02_client.cpp -lboost_system -lpthread
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: echo_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        while (true)
        {
            char request[1024] = {0};
            std::cout << "Enter message: ";
            std::cin.getline(request, 1024);

            boost::asio::write(socket, boost::asio::buffer(request, strlen(request)));

            char reply[1024];
            size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, strlen(request)));
            std::cout << "Reply is: ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}