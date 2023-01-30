#ifndef HTTP_HANDLER_FACTORY_H
#define HTTP_HANDLER_FACTORY_H

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>

class HTTPHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    HTTPHandlerFactory() {};
    ~HTTPHandlerFactory() {};
    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
};

#endif // !HTTP_HANDLER_FACTORY_H