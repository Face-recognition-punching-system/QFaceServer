#include <Poco/JSON/Parser.h>
#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>
#include <format>

#include "worker_http_handler.h"
#include "database_pool.h"
#include "redis_pool.h"
#include "logger.h"
#include "utils.h"

void WorkerSignInRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	try {
		Poco::JSON::Parser parser;
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->has("workerId") && object->has("password") && object->size() == 2) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string&& workerId = object->get("workerId");
			std::string&& password = object->get("password");
			std::string query = std::format(
				"select id, workerId, name, age, department from qface.worker where workerId = \"{}\" and password = \"{}\"",
				workerId, password);
			body = db->readOne(query, { "id", "workerId", "name", "age", "department"});
			auto ret = parser.parse(body).extract<Poco::JSON::Object::Ptr>();
			if (ret->has("id")) {
				std::string id = ret->get("id");
				Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();
				Poco::UUID uuid(generator.create());
				std::string token = uuid.toString();
				auto rp = RedisPool::getInstance();
				if (!rp->set(id, token)) {
					body = utils::body("unknown error");
				} else {
					ret->set("token", token);
					std::stringstream ostream;
					ret->stringify(ostream);
					body = ostream.str();
				}
			} else {
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

void WorkerGetClockHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->has("id") && object->size() == 1) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string id = object->get("id");
			std::string query = std::format(
				"SELECT clock.id as cid, create_time, img from worker RIGHT join clock on worker.id = clock.wid where worker.id = \"{}\"", id);
			body = db->read(query,
				{ "cid", "create_time", "img" });
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

void WorkerCreateFeedbackHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->has("id") && object->size() == 1) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string id = object->get("id");
			std::string query = std::format("INSERT INTO qface.feedback (cid, create_time, reason, res) VALUES({}, CURRENT_TIMESTAMP(3), 'null', 'нч')", id);
			body = db->create(query);
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

void WorkerGetFeedbackHandler::handleRequest(
	Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& res) {
	std::string body("");
	res.setContentType("application/json");
	std::string data(std::istreambuf_iterator<char>(req.stream()), {});
	Poco::JSON::Parser parser;
	try {
		auto object = parser.parse(data).extract<Poco::JSON::Object::Ptr>();
		if (object->has("id") && object->size() == 1) {
			DatabasePool* dbp = DatabasePool::getInstance();
			std::shared_ptr<Database> db = dbp->getDatabase();
			std::string id = object->get("id");
			std::string query = std::format(
				"SELECT feedback.id as fid, clock.id as cid, reason, feedback.create_time, res from worker RIGHT join clock on worker.id = clock.wid RIGHT JOIN feedback  on clock.id = feedback.cid where worker.id = \"{}\"", id);
			body = db->read(query,
				{ "fid", "cid", "reason", "create_time", "res" });
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