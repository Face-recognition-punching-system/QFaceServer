#include "http_handler_factory.h"
#include "admin_http_handler.h"
#include "video_http_handler.h"
#include "worker_http_handler.h"
#include "intercept_http_handler.h"

Poco::Net::HTTPRequestHandler* HTTPHandlerFactory::createRequestHandler(
	const Poco::Net::HTTPServerRequest& req) {
	const std::string method = req.getMethod().c_str();
	const std::string uri = req.getURI().c_str();
	const std::string contentType = req.getContentType();
	std::cout << method << " " << uri << " " << contentType << std::endl;
	if (uri == "/admin/signIn" && method == "POST" && contentType == "application/json") {
		return new AdminSignInRequestHandler;
	} else if (uri == "/admin/updatePassword" && method == "POST" && contentType == "application/json") {
		return new AdminUpdatePasswordRequestHandler;
	} else if (uri == "/admin/workerInfo" && method == "GET" && contentType == "application/json") {
		return new AdminGetWorkerInfoRequestHandler;
	} else if (uri == "/admin/clock" && method == "GET" && contentType == "application/json") {
		return new AdminGetClockRequestHandler;
	} else if (uri == "/admin/notClock" && method == "GET" && contentType == "application/json") {
		return new AdminGetNotClockRequestHandler;
	} else if (uri == "/admin/workerClock" && method == "POST" && contentType == "application/json") {
		return new AdminGetWorkerClockRequestHandler;
	} else if (uri == "/admin/feedback" && method == "GET" && contentType == "application/json") {
		return new AdminGetFeedbackRequestHandler;
	} else if (uri == "/admin/feedbackRes" && method == "GET" && contentType == "application/json") {
		return new AdminGetFeedbackResRequestHandler;
	} else if (uri == "/admin/updateFeedback" && method == "POST" && contentType == "application/json") {
		return new AdminUpdateFeedbackRequestHandler;
	} else if (uri == "/admin/getWorkerImg" && method == "POST" && contentType == "application/json") {
		return new AdminGetWorkerImgRequestHandler;
	} else if (uri == "/admin/updateWorkerImg" && method == "POST" && contentType == "application/json") {
		return new AdminUpdateWorkerImgRequestHandler;
	} else if (uri == "/video/reco" && method == "POST" && contentType == "application/json") {
		return new VideoRecoHandler;
	} else if (uri == "/worker/signIn" && method == "POST" && contentType == "application/json;charset=UTF-8") {
		return new WorkerSignInRequestHandler;
	} else if (uri == "/worker/getClock" && method == "POST" && contentType == "application/json;charset=UTF-8") {
		return new WorkerGetClockHandler;
	} else if (uri == "/worker/getFeedback" && method == "POST" && contentType == "application/json;charset=UTF-8") {
		return new WorkerGetFeedbackHandler;
	} else if (uri == "/worker/addFeedback" && method == "POST" && contentType == "application/json;charset=UTF-8") {
		return new WorkerCreateFeedbackHandler;
	} else {
		return new InterceptHttpHandler;
	}
}
