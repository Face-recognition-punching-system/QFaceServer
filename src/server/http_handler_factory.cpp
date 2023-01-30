#include "http_handler_factory.h"
#include "admin_http_handler.h"
#include "user_http_handler.h"
#include "intercept_http_handler.h"

Poco::Net::HTTPRequestHandler* HTTPHandlerFactory::createRequestHandler(
    const Poco::Net::HTTPServerRequest& request){
    const std::string method = std::move(request.getMethod().c_str());
    const std::string uri = std::move(request.getURI().c_str());
    if (uri == "/user/login" && method == "POST") {
        return new UserLoginRequestHandler;
    }else if(uri == "/usr/reco" && method == "POST") {
        return new UserRecoRequestHandler;
    }else {
        return new InterceptHttpHandler;
    }
}
