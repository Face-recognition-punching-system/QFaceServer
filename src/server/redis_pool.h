#ifndef REDIS_H
#define REDIS_H

#include <memory>
#include <format>
#include <sw/redis++/redis++.h>
#include <sw/redis++/sentinel.h>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>

class RedisPool {
private:
	static std::shared_ptr<RedisPool> _instance;
	sw::redis::ConnectionOptions _connection_options;
	sw::redis::ConnectionPoolOptions _pool_options;
	sw::redis::Redis* _redis;
	bool _status;
	static void destroyInstance(RedisPool*);
	RedisPool();
	~RedisPool();
	bool initialize();
	
public:
	static std::shared_ptr<RedisPool> getInstance();
	bool set(std::string&, std::string&);
	std::string get(std::string&);
	bool del(std::string&);
};

#endif // REDIS_H




//#ifndef REDIS_H
//#define REDIS_H

//#include <Poco/Redis/Client.h>
//#include <Poco/Redis/Command.h>
//#include <mutex>
//#include <memory>

//class Redis {
//private:
//	Poco::Redis::Client *_redis;
//	clock_t _alivetime;
//
//public:
//	Redis();
//	~Redis();
//	bool connect();
//	bool createData(std::string&&, std::string&&);
//	std::string readData(std::string&);
//	bool delData(std::string&&);
//	inline clock_t getAliveTime() const {
//		return clock() - _alivetime;
//	}
//	inline void refreshAliveTime() {
//		_alivetime = clock();
//	}
//};

//#endif // !REDIS_H