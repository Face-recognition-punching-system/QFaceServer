#include <format>

#include "redis_pool.h"
#include "logger.h"
#include "../config/config.h"

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
	} catch (const sw::redis::ReplyError& err) {
		logger->error(std::format("RedisHandler-- ReplyError£º%s {}", err.what()));
		return false;
	} catch (const sw::redis::TimeoutError& err) {
		logger->error(std::format("RedisHandler-- TimeoutError%s \n", err.what()));
		return false;
	} catch (const sw::redis::ClosedError& err) {
		logger->error(std::format("RedisHandler-- ClosedError%s \n", err.what()));
		return false;
	} catch (const sw::redis::IoError& err) {
		logger->error(std::format("RedisHandler-- IoError%s \n", err.what()));
		return false;
	} catch (const sw::redis::Error& err) {
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
		_redis->del(key);
		return _redis->set(key, value, std::chrono::seconds(241920), sw::redis::UpdateType::ALWAYS);
	} else {
		return _redis->set(key, value, std::chrono::seconds(241920), sw::redis::UpdateType::ALWAYS);
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
	} else {
		return "";
	}
}


bool RedisPool::del(std::string& key) {
	if (!_status) {
		return false;
	}

	if (_redis->del(key)) {
		return true;
	} else {
		return false;
	}
}
