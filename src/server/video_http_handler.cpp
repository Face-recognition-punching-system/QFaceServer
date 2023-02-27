#include <Poco/JSON/Parser.h>
#include <memory>
#include <opencv2/opencv.hpp>
#include <regex>

#include "video_http_handler.h"
#include "utils.h"
#include "logger.h"
#include "database_pool.h"
#include "../arcface/detector_handler.h"
#include "../arcface/facenet_handler.h"

void VideoRecoHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	//res.add("access-control-allow-origin", "*");
	//res.add("access-control-allow-methods", "post,get,options,delete");
	//res.add("access-control-max-age", "3600");
	//res.add("access-control-allow-headers", "x-requested-with,content-type");
	//res.add("access-control-allow-credentials", "true");
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
					if (str == "none") {
						body = utils::body("fail");
					} else {
						std::regex parttern("^\\D*(\\d+)\\D*$");
						std::string id = std::regex_replace(str, parttern, "$1");
						if (id != "") {
							DatabasePool* dbp = DatabasePool::getInstance();
							std::shared_ptr<Database> db = dbp->getDatabase();
							std::string query = std::format(
								"select name, workerId from qface.worker where id = \"{}\"", id);
							body = db->readOne(query, { "name", "workerId"});
							auto ret = parser.parse(body).extract<Poco::JSON::Object::Ptr>();
							if (!ret->has("name") || !ret->has("workerId")) {
								body = utils::body("fail");
							}
						} else {
							body = utils::body("fail");
						}
					}
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