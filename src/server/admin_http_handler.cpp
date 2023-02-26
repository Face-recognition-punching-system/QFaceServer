#include "admin_http_handler.h"
#include "database_pool.h"
#include "redis_pool.h"
#include "logger.h"
#include "utils.h"
#include "../arcface/detector_handler.h"
#include "../arcface/facenet_handler.h"

void AdminSignInRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	try {
		Poco::JSON::Parser parser;
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->has("adminId") && object->has("password") && object->has("csrfToken") && object->size() == 3) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string&& adminId = object->get("adminId");
			std::string&& password = object->get("password");
			std::string query = std::format(
				"select id from qface.admin where adminId = \"{}\" and password = \"{}\"",
				adminId, password);
			body = db->readOne(query, { "id" });
			auto ret = parser.parse(body).extract<Poco::JSON::Object::Ptr>();
			if (ret->has("id")) {
				std::string id = ret->get("id");
				std::string csrfToken = object->get("csrfToken");
				auto rp = RedisPool::getInstance();
				if (!rp->set(id, csrfToken)) {
					body = utils::body("unknown error");
				}
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

void AdminUpdatePasswordRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	try {
		Poco::JSON::Parser parser;
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (!object->has("csrfToken")) {
			body = utils::body("authentication failure");
		} else if (object->has("password") && object->has("id") && object->size() == 3) {
			std::string id = object->get("id");
			std::string csrfToken = object->get("csrfToken");
			auto rp = RedisPool::getInstance();
			std::string res = rp->get(id);
			if (res != "" && res == csrfToken) {
				body = utils::body("unknown error");
				DatabasePool* dbp = DatabasePool::getInstance();
				std::shared_ptr<Database> db = dbp->getDatabase();
				std::string password = object->get("password");
				std::string query = std::format(
					"update qface.admin set password = \"{}\" where id = \"{}\"",
					password, id);
				body = db->update(query);
			} else {
				body = utils::body("signOut");
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

void AdminGetWorkerInfoRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->size() == 0) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string query = "select id, workerId, name, age, department from qface.worker";
			body = db->read(query,
				{ "id", "workerId", "name", "age", "department" });
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

void AdminGetClockRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->size() == 0) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string query = "select workerid, name, create_time from clock left join worker on clock.wid = worker.id where to_days(create_time) = to_days(now())";
			body = db->read(query,
				{ "workerId", "name", "create_time" });
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

void AdminGetNotClockRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->size() == 0) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string query = "select workerId, name from worker where id not in(select wid as id from clock where (to_days(create_time) = to_days(now())))";
			body = db->read(query,
				{ "workerId", "name" });
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

void AdminGetWorkerClockRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	std::cout << data.c_str() << std::endl;
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (!object->has("csrfToken")) {
			body = utils::body("authentication failure");
		} else if (object->has("wid") && object->has("id") && object->size() == 3) {
			std::string csrfToken = object->get("csrfToken");
			std::string id = object->get("id");
			auto rp = RedisPool::getInstance();
			std::string res = rp->get(id);
			if (res != "" && res == csrfToken) {
				DatabasePool* dbp = DatabasePool::getInstance();
				std::shared_ptr<Database> db = dbp->getDatabase();
				std::string wid = object->get("wid");
				std::string query = std::format(
					"SELECT id, create_time, img, isClock FROM qface.clock where wid = \"{}\"", wid);
				body = db->read(query,
					{ "id", "create_time", "img", "isClock" });
			} else {
				body = utils::body("signOut");
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

void AdminGetFeedbackRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->size() == 0) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string query = "SELECT feedback.id as fid, clock.id as cid, workerId, name, reason, feedback.create_time, res from worker RIGHT join clock on worker.id = clock.wid RIGHT JOIN feedback  on clock.id = feedback.cid where res = \"无\"";
			body = db->read(query,
				{ "fid", "cid", "workerId", "name", "reason", "create_time", "res" });
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

void AdminUpdateFeedbackRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (!object->has("csrfToken")) {
			body = utils::body("authentication failure");
		} else if (object->has("id") && object->has("fid") && object->has("cid") && object->has("res") && object->size() == 5) {
			std::string csrfToken = object->get("csrfToken");
			std::string id = object->get("id");
			auto rp = RedisPool::getInstance();
			std::string res = rp->get(id);
			if (res != "" && res == csrfToken) {
				DatabasePool* dbp = DatabasePool::getInstance();
				std::shared_ptr<Database> db = dbp->getDatabase();
				const std::string&& res = utils::U2G(object->get("res"));
				const std::string&& fid = object->get("fid");
				const std::string&& cid = object->get("cid");
				const std::string&& isClock = res == "通过" ? "是" : "否";
				std::string query = std::format(
					"update feedback set res = \"{}\" where id = \"{}\"", res, fid);
				body = db->update(query);
				query = std::format(
					"update clock set isClock = \"{}\" where id = \"{}\"", isClock, cid);
				body = db->update(query);
			} else {
				body = utils::body("signOut");
			}
		} else {
			body = utils::body("param invalid");
		}

		res.sendBuffer(body.c_str(), body.length());
	} catch (const Poco::JSON::JSONException& e) {
		auto instance = Logger::getInstance();
		instance->information(e.displayText());
		instance->information("\n");
		body = utils::body("json parser error");
	}
}

void AdminGetFeedbackResRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->size() == 0) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string query = "SELECT workerId, name, reason, feedback.create_time, res from worker RIGHT join clock on worker.id = clock.wid RIGHT JOIN feedback on clock.id = feedback.cid where res != \"无\"";
			body = db->read(query,
				{ "workerId", "name", "reason", "create_time", "res" });
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

void AdminGetWorkerImgRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (!object->has("csrfToken")) {
			body = utils::body("authentication failure");
		}
		if (object->has("id") && object->has("wid") && object->size() == 3) {
			std::string csrfToken = object->get("csrfToken");
			std::string id = object->get("id");
			auto rp = RedisPool::getInstance();
			std::string res = rp->get(id);
			if (res != "" && res == csrfToken) {
				std::string wid = object->get("wid");
				std::string path = std::format("img/{}.jpg", wid);
				std::cout << path << std::endl;
				try {
					if (_access(path.c_str(), 0) == -1) {
						body = utils::body("unknown error");
					} else {
						cv::Mat img = cv::imread(path);
						if (!img.empty()) {
							std::string base64 = utils::Mat2Base64(img, "jpg");
							Poco::JSON::Object o;
							std::stringstream ostream;
							o.set("img", base64);
							o.stringify(ostream);
							body = ostream.str();
						} else {
							body = utils::body("unknown error");
						}
					}
				} catch (cv::Exception& e) {
					e.formatMessage();
					body = utils::body("unknown error");
				}
			} else {
				body = utils::body("signOut");
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

void AdminUpdateWorkerImgRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (!object->has("csrfToken")) {
			body = utils::body("authentication failure");
		}
		if (object->has("id") && object->has("wid") && object->has("img") && object->size() == 4) {
			std::string csrfToken = object->get("csrfToken");
			std::string id = object->get("id");
			auto rp = RedisPool::getInstance();
			std::string res = rp->get(id);
			if (res != "" && res == csrfToken) {
				std::string wid = object->get("wid");
				std::string base64 = object->get("img");
				std::string path = std::format("img/{}.jpg", wid);
				try {
					std::string prefix = "img/";
					if (_access(prefix.c_str(), 0) == -1) {
						std::unique_lock<std::mutex> lock(_mutex);
						if (_access(prefix.c_str(), 0) == -1) {
							_mkdir(prefix.c_str());
						}
					}

					cv::Mat img = utils::Base2Mat(base64);
					std::unique_lock<std::mutex> lock(_mutex);
					auto detectorHandler = DetectorHandler::getInstance();
					auto facenetHandler = FacenetHandler::getInstance();
					cv::Mat face = detectorHandler->detect(img);
					if (face.empty()) {
						body = utils::body("fail");
					} else {
						cv::imwrite(path, face);
						body = utils::body("success");
					}
				} catch (cv::Exception& e) {
					e.formatMessage();
					body = utils::body("unknown error");
				}
			} else {
				body = utils::body("signOut");
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