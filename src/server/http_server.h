#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>

class HttpApplication : public Poco::Util::ServerApplication {
private:
    Poco::Net::HTTPServer* _httpSvr;

protected:
    void initialize(Poco::Util::Application& self);
    void uninitialize();
    int main(const std::vector<std::string>& args);

public:
    HttpApplication();
    ~HttpApplication();
};

#endif // !HTTP_SERVER_H
