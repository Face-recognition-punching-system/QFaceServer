#ifndef VIDEO_HTTP_HANDLER_H
#define VIDEO_HTTP_HANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>

class VideoRecoHandler : public Poco::Net::HTTPRequestHandler {
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
};

#endif // !VIDEO_HTTP_HANDLER_H