#ifndef USER_HTTP_HANDLER_H
#define USER_HTTP_HANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>

class WorkerSignInRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class WorkerGetClockHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class WorkerCreateFeedbackHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class WorkerGetFeedbackHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

#endif // !USER_HTTP_HANDLER_H