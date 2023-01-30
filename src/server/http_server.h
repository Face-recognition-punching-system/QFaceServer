#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>

class HttpApplication : public Poco::Util::ServerApplication {
private:
    //http监听服务
    Poco::Net::HTTPServer* _httpSvr;

protected:
    //初始化操作
    void initialize(Poco::Util::Application& self);
    //清理操作
    void uninitialize();
    //主函数入口
    int main(const std::vector<std::string>& args);

public:
    HttpApplication();
    ~HttpApplication();
};

#endif // !HTTP_SERVER_H
