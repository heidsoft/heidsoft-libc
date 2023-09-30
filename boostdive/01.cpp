#include <boost/asio.hpp>
using boost::asio::ip::tcp;
//g++ -o 01 01.cpp -lboost_system -lpthread
int main() {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));
    for (;;) {
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        std::string message = "Hello, World!\n";
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
    return 0;
}