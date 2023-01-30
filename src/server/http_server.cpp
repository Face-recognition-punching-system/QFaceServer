#include "http_server.h"
#include "http_handler_factory.h"
#include "database.h"

HttpApplication::HttpApplication() {
    _httpSvr = nullptr;
    logger().information("starting up");
}

HttpApplication::~HttpApplication() {
    Database::destroyInstance();
    logger().information("shutting down");
}

void HttpApplication::initialize(Poco::Util::Application& self) {
    loadConfiguration();
    Poco::Util::ServerApplication::initialize(self);
    logger().information("initialize");
}

void HttpApplication::uninitialize() {
    Poco::Util::ServerApplication::uninitialize();
    logger().information("uninitialize");
}

int HttpApplication::main(const std::vector<std::string>& args){
    Poco::UInt16 port = 8888;
    Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
    pParams->setMaxQueued(10);
    pParams->setMaxThreads(2);
    Poco::Net::ServerSocket svs(port);
    Poco::Net::HTTPServer* _httpSvr = new Poco::Net::HTTPServer(new HTTPHandlerFactory(), svs, pParams);
    _httpSvr->start();
    Poco::Util::ServerApplication::waitForTerminationRequest();
    _httpSvr->stop();
    return Poco::Util::ServerApplication::EXIT_OK;
}