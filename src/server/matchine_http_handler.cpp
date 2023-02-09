#include "matchine_http_handler.h"
#include "utils.h"

void MatchineHttpHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	const std::string body = utils::body("api is error");
	res.sendBuffer(body.c_str(), body.length());
}