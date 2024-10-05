#include <boost/asio.hpp>
//#include <functional>  // 包含 std::bind 的头文件#include <boost/bind.hpp>
#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;

// 创建一个session类，用于处理每个连接
class session
{
public:
    // 构造函数，接收一个io_service对象
    session(boost::asio::io_service& io_service)
        : socket_(io_service)
    {
    }

    // 返回socket对象的引用
    tcp::socket& socket()
    {
        return socket_;
    }

    // 开始读取数据
    void start()
    {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&session::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    // 读取数据后的处理函数
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred)
    {
        if (!error)
        {
            // 如果读取成功，将数据写回客户端
            boost::asio::async_write(socket_,
                boost::asio::buffer(data_, bytes_transferred),
                boost::bind(&session::handle_write, this,
                    boost::asio::placeholders::error));
        }
        else
        {
            // 如果读取失败，删除当前对象
            delete this;
        }
    }

    // 写数据后的处理函数
    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            // 如果写入成功，继续读取数据
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                boost::bind(&session::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            // 如果写入失败，删除当前对象
            delete this;
        }
    }

private:
    tcp::socket socket_;  // socket对象，用于数据传输
    enum { max_length = 1024 };  // 定义最大读取数据长度
    char data_[max_length];  // 数据缓冲区
};

// 创建一个server类，用于接收连接并创建session对象
class server
{
public:
    // 构造函数，接收一个io_service对象和监听的端口号
    server(boost::asio::io_service& io_service, short port)
        : io_service_(io_service),
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        start_accept();  // 开始接收连接
    }

private:
    // 开始接收连接
    void start_accept()
    {
        session* new_session = new session(io_service_);
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this, new_session,
                boost::asio::placeholders::error));
    }

    // 接收连接后的处理函数
    void handle_accept(session* new_session,
        const boost::system::error_code& error)
    {
        if (!error)
        {
            // 如果接收成功，开始处理连接
            new_session->start();
        }
        else
        {
            // 如果接收失败，删除session对象
            delete new_session;
        }

        start_accept();  // 继续接收新的连接
    }

    boost::asio::io_service& io_service_;  // io_service对象，用于异步I/O操作
    tcp::acceptor acceptor_;  // acceptor对象，用于接收连接
};

// 主函数
// g++ -o 02_server 02_server.cpp -lboost_system -lpthread^#include <boost/bind.hpp>
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        using namespace std; // For atoi.
        server s(io_service, atoi(argv[1]));

        io_service.run();  // 运行io_service对象，开始异步I/O操作
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}