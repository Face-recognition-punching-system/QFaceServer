#include "redis_pool.h"
#include "../config/config.h"
#include "logger.h"

std::shared_ptr<RedisPool> RedisPool::_instance(new(std::nothrow) RedisPool(), RedisPool::destroyInstance);

RedisPool::RedisPool() {
	_redis = nullptr;
	_status = initialize();
}

RedisPool::~RedisPool() {
	delete _redis;
	_redis = nullptr;
}

bool RedisPool::initialize() {
	auto logger = Logger::getInstance();
	_connection_options.host = host;
	_connection_options.port = redisPort;
	_connection_options.password = redisPassWord;
	_connection_options.db = 1;
	_connection_options.socket_timeout = std::chrono::milliseconds(200);
	_pool_options.size = 2;
	_pool_options.wait_timeout = std::chrono::milliseconds(100);
	_pool_options.connection_lifetime = std::chrono::minutes(10);
	try {
		_redis = new sw::redis::Redis(_connection_options, _pool_options);
	}
	catch (const sw::redis::ReplyError& err) {
		logger->error(std::format("RedisHandler-- ReplyError£º%s {}", err.what()));
		return false;
	}
	catch (const sw::redis::TimeoutError& err) {
		logger->error(std::format("RedisHandler-- TimeoutError%s \n", err.what()));
		return false;
	}
	catch (const sw::redis::ClosedError& err) {
		logger->error(std::format("RedisHandler-- ClosedError%s \n", err.what()));
		return false;
	}
	catch (const sw::redis::IoError& err) {
		logger->error(std::format("RedisHandler-- IoError%s \n", err.what()));
		return false;
	}
	catch (const sw::redis::Error& err) {
		logger->error(std::format("RedisHandler-- other% s \n", err.what()));
		return false;
	}

	return true;
}

std::shared_ptr<RedisPool> RedisPool::getInstance() {
	return _instance;
}

void RedisPool::destroyInstance(RedisPool* instance) {
	delete instance;
}

bool RedisPool::set(std::string& key, std::string& value) {
	if (!_status) {
		return "";
	}

	if (_redis->exists(key)) {
		return _redis->set(key, value, std::chrono::days(7), sw::redis::UpdateType::EXIST);
	}
	else {
		return _redis->set(key, value, std::chrono::days(7), sw::redis::UpdateType::NOT_EXIST);
	}
	
}

std::string RedisPool::get(std::string& key) {
	if (!_status) {
		return "";
	}

	auto res = _redis->get(key);
	if (res) {
		std::string ret = *res;
		return ret;
	}
	else {
		return "";
	}
}


bool RedisPool::del(std::string& key) {
	if (!_status) {
		return false;
	}

	if (_redis->del(key)) {
		return true;
	}
	else {
		return false;
	}
}




//#include "redis.h"
//#include "logger.h"
//#include "../config/config.h"
//
//bool sendAuth(Poco::Redis::Client* client, const std::string& pwd) {
//	Poco::Redis::Array cmd;
//	cmd << "AUTH" << pwd;
//	std::string response;
//	auto logger = Logger::getInstance();
//	try {
//		response = client->execute<std::string>(cmd);
//	}
//	catch (const Poco::Exception& error) {
//		logger->information(std::format("%s\n", error.displayText().c_str()));
//		return  false;
//	}
//
//	return (response == "OK");
//}
//
//Redis::Redis() {
//	auto logger = Logger::getInstance();
//	logger->information("redis initialize");
//}
//
//Redis::~Redis() {
//	auto logger = Logger::getInstance();
//	logger->information("redis uninitialize");
//}
//
//bool Redis::connect() {
//	auto logger = Logger::getInstance();
//	try {
//		_redis = new Poco::Redis::Client(host, redisPort);
//		if (!sendAuth(_redis, redisPassWord)) {
//			logger->information("redis connect error");
//			logger->information("authentication failure!\n");
//			return false;
//		}
//		else {
//			logger->information("redis connect success\n");
//			return true;
//		}
//	}
//	catch (Poco::BadCastException& error) {
//		logger->information("redis connect error");
//		logger->information(std::format("{}\n", error.displayText().c_str()));
//		return false;
//	}
//	catch (Poco::Redis::RedisException& error) {
//		logger->information("redis connect error");
//		logger->information(std::format("{}\n", error.displayText().c_str()));
//		return false;
//	}
//}
//
//bool Redis::createData(std::string&& key, std::string&& value) {
//	auto logger = Logger::getInstance();
//	Poco::Redis::Command execute = Poco::Redis::Command::set(key, value);
//	try {
//		auto ret = _redis->execute<std::string>(execute);
//		std::cout << ret << std::endl;
//		return ret == "OK";
//	}
//	catch (Poco::Redis::RedisException& error) {
//		if (_redis->isConnected()) {
//			logger->information(std::format("{}\n", error.displayText().c_str()));
//			return false;
//		}
//		else {
//			if (connect()) {
//				auto ret = _redis->execute<std::string>(execute);
//				return ret == "OK";
//			}
//			else {
//				return false;
//			}
//		}
//	}
//}
//
//std::string Redis::readData(std::string& key) {
//	auto logger = Logger::getInstance();
//	Poco::Redis::Command execute = Poco::Redis::Command::get(key);
//	try {
//		auto ret = _redis->execute<Poco::Redis::BulkString>(execute);
//		return ret.value();
//	}
//	catch (Poco::Redis::RedisException& error) {
//		if (_redis->isConnected()) {
//			logger->information(std::format("{}\n", error.displayText().c_str()));
//			return "";
//		}
//		else {
//			if (connect()) {
//				auto ret = _redis->execute<Poco::Redis::BulkString>(execute);
//				return ret.value();
//			}
//			else {
//				return "";
//			}
//		}
//	}
//}
//
//bool Redis::delData(std::string&& key) {
//	auto logger = Logger::getInstance();
//	Poco::Redis::Command execute = Poco::Redis::Command::del(key);
//	try {
//		auto ret = _redis->execute<std::string>(execute);
//		std::cout << ret << std::endl;
//		return ret == "OK";
//	}
//	catch (Poco::Redis::RedisException& error) {
//		if (_redis->isConnected()) {
//			logger->information(std::format("{}\n", error.displayText().c_str()));
//			return false;
//		}
//		else {
//			if (connect()) {
//				auto ret = _redis->execute<std::string>(execute);
//				return ret == "OK";
//			}
//			else {
//				return false;
//			}
//		}
//	}
//}