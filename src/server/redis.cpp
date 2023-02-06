#include "redis.h"
#include "logger.h"
#include "../config/config.h"

bool sendAuth(Poco::Redis::Client* client, const std::string& pwd) {
	Poco::Redis::Array cmd;
	cmd << "AUTH" << pwd;
	std::string response;
	auto logger = Logger::getInstance();
	try {
		response = client->execute<std::string>(cmd);
	}catch (const Poco::Exception& error) {
		logger->information(std::format("%s\n", error.displayText().c_str()));
		return  false;
	}

	return (response == "OK");
}

Redis* Redis::_instance(nullptr);

std::recursive_mutex Redis::_mutex;

Redis::Redis() {
	auto logger = Logger::getInstance();
	logger->information("redis initialize\n");
	connect();
}

Redis::~Redis() {
	auto logger = Logger::getInstance();
	logger->information("redis uninitialize\n");
	if (_redis->isConnected()) {
		_redis->disconnect();
	}

	delete _redis;
	_redis = nullptr;
}

bool Redis::connect() {
	auto logger = Logger::getInstance();
	try {
		_redis = new Poco::Redis::Client(host, redisPort);
		if (!sendAuth(_redis, redisPassWord)) {
			logger->information("redis connect error");
			logger->information("authentication failure!\n");
			return false;
		}
		else {
			logger->information("redis connect success\n");
			return true;
		}
	}
	catch (Poco::BadCastException& error) {
		logger->information("redis connect error");
		logger->information(std::format("{}\n", error.displayText().c_str()));
		return false;
	}
	catch (Poco::Redis::RedisException& error) {
		logger->information("redis connect error");
		logger->information(std::format("{}\n", error.displayText().c_str()));
		return false;
	}
}

Redis* Redis::getInstance() {
	if (_instance == nullptr) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (_instance == nullptr) {
			_instance = new(std::nothrow) Redis();
		}
	}

	return _instance;
}

void Redis::destoryInstance() {
	if (_instance != nullptr) {
		delete _instance;
		_instance = nullptr;
	}
}

bool Redis::createData(std::string&& key, std::string&& value) {
	std::unique_lock<std::recursive_mutex> lock(_mutex);
	auto logger = Logger::getInstance();
	Poco::Redis::Command set = Poco::Redis::Command::set(key, value);
	try {
		auto ret = _redis->execute<std::string>(set);
		return ret == "OK";
	}
	catch (Poco::Redis::RedisException& error) {
		if (_redis->isConnected()) {
			logger->information(std::format("{}\n", error.displayText().c_str()));
			return false;
		}
		else {
			if (connect()) {
				auto ret = _redis->execute<std::string>(set);
				return ret == "OK";
			}
			else {
				return false;
			}
		}
	}
}

std::string Redis::readData(std::string& key) {
	std::unique_lock<std::recursive_mutex> lock(_mutex);
	auto logger = Logger::getInstance();
	Poco::Redis::Command execute = Poco::Redis::Command::get(key);
	try {
		auto ret = _redis->execute<Poco::Redis::BulkString>(execute);
		return ret.value();
	}
	catch (Poco::Redis::RedisException& error) {
		if (_redis->isConnected()) {
			logger->information(std::format("{}\n", error.displayText().c_str()));
			return "";
		}
		else {
			if (connect()) {
				auto ret = _redis->execute<Poco::Redis::BulkString>(execute);
				return ret.value();
			}
			else {
				return "";
			}
		}
	}
}

bool Redis::delData(std::string&& key) {
	std::unique_lock<std::recursive_mutex> lock(_mutex);
	auto logger = Logger::getInstance();
	Poco::Redis::Command execute = Poco::Redis::Command::del(key);
	try {
		auto ret = _redis->execute<std::string>(execute);
		std::cout << ret << std::endl;
		return ret == "OK";
	}
	catch (Poco::Redis::RedisException& error) {
		if (_redis->isConnected()) {
			logger->information(std::format("{}\n", error.displayText().c_str()));
			return false;
		}
		else {
			if (connect()) {
				auto ret = _redis->execute<std::string>(execute);
				return ret == "OK";
			}
			else {
				return false;
			}
		}
	}
}