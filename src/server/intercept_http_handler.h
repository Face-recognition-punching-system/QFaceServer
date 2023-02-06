#ifndef INTERCEPT_HTTP_HANDLER_H
#define INTERCEPT_HTTP_HANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>

class InterceptHttpHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

#endif // !INTERCEPT_HTTP_HANDLER_H
