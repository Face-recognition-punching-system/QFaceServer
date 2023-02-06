#include "intercept_http_handler.h"
#include "utils.h"

void InterceptHttpHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	const std::string body = utils::body("api is error");
	res.sendBuffer(body.c_str(), body.length());
}