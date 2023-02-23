#ifndef ADMIN_HTTP_HANDLER_H
#define ADMIN_HTTP_HANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h> 
#include <Poco/JSON/Parser.h>
#include <format>
#include <hiredis/hiredis.h>
#include <io.h>
#include <direct.h>
#include <mutex>
#include <opencv2/opencv.hpp>

class AdminSignInRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminUpdatePasswordRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminGetWorkerInfoRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminGetClockRequestHandler :public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminGetNotClockRequestHandler :public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminGetWorkerClockRequestHandler :public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminGetFeedbackRequestHandler :public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminUpdateFeedbackRequestHandler :public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminGetFeedbackResRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminGetWorkerImgRequestHandler : public Poco::Net::HTTPRequestHandler {
private:
	std::mutex _mutex;

public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

class AdminUpdateWorkerImgRequestHandler : public Poco::Net::HTTPRequestHandler {
private:
	std::mutex _mutex;

public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

#endif // !ADMIN_HTTP_HANDLER_H
