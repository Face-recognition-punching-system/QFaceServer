#include <Poco/JSON/Parser.h>
#include <memory>
#include <opencv2/opencv.hpp>
#include <regex>

#include "video_http_handler.h"
#include "utils.h"
#include "logger.h"
#include "../arcface/detector_handler.h"
#include "../arcface/facenet_handler.h"

void VideoRecoHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	/**
	 * 	res.add("Access-Control-Allow-Origin", "*");
	 *  res.add("Access-Control-Allow-Methods", "POST,GET,OPTIONS,DELETE");
	 *  res.add("Access-Control-Max-Age", "3600");
	 *  res.add("Access-Control-Allow-Headers", "x-requested-with,content-type");
	 *  res.add("Access-Control-Allow-Credentials", "true");
	 */
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	try {
		Poco::JSON::Parser parser;
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		auto facenetHandler = FacenetHandler::getInstance();
		auto detectorHandler = DetectorHandler::getInstance();
		if (object->has("img") && object->size() == 1) {
			try {
				std::string base64 = object->get("img");
				cv::Mat img = utils::Base2Mat(base64);
				cv::Mat face = detectorHandler->detect(img);
				if (face.empty()) {
					body = utils::body("fail");
				} else {
					std::string str = facenetHandler->faceRecognition(face);
					body = utils::body(str.c_str());
				}
			} catch (cv::Exception& e) {
				e.formatMessage();
				body = utils::body("unknown error");
			}
		} else {
			body = utils::body("param invalid");
		}
	} catch (const Poco::JSON::JSONException& e) {
		auto instance = Logger::getInstance();
		instance->information(e.displayText());
		instance->information("\n");
		body = utils::body("json parser error");
	}

	res.sendBuffer(body.c_str(), body.length());
}