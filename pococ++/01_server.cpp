#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include "Poco/Util/ServerApplication.h"

// 定义一个请求处理器
class MyRequestHandler: public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("text/html");

        std::ostream& ostr = response.send();
        ostr << "<html><head><title>HTTP Server powered by POCO C++</title>";
        ostr << "<body><h1>Welcome to HTTP Server powered by POCO C++!</h1></body></html>";
    }
};

// 定义一个请求处理器工厂
class MyRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&)
    {
        return new MyRequestHandler;
    }
};


class MyServerApplication : public Poco::Util::ServerApplication
{
protected:
    int main(const std::vector<std::string>& args)
    {
        // 设置服务器的监听端口
        Poco::Net::ServerSocket svs(8080);

        // 创建一个新的 HTTPServerParams 对象
        Poco::Net::HTTPServerParams::Ptr params(new Poco::Net::HTTPServerParams);

        // 创建HTTP服务器
        Poco::Net::HTTPServer srv(new MyRequestHandlerFactory, svs, params);

        // 开始服务器
        srv.start();

        // 让服务器持续运行，直到接收到终止信号
        waitForTerminationRequest();  // 正确的调用方式

        // 停止服务器
        srv.stop();

        return Application::EXIT_OK;
    }
};


// 主函数
// g++ -o myhttpserver 01_server.cpp -lPocoNet -lPocoUtil -lPocoFoundationg++ -o myhttpserver 01_server.cpp -lPocoNet
int main(int argc, char** argv)
{
    MyServerApplication app;
    return app.run(argc, argv);
    return 0;
}