#ifndef MATCHINE_HTTP_HANDLER_H
#define MATCHINE_HTTP_HANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/JSON/Parser.h>

class MatchineHttpHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

#endif
