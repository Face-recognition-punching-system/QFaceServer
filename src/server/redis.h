#ifndef REDIS_H
#define REDIS_H

#include <Poco/Redis/Client.h>
#include <Poco/Redis/Command.h>

class Redis {
private:
	static Redis* _instance;
	static std::recursive_mutex _mutex;
	Poco::Redis::Client *_redis;
	Redis();
	~Redis();
	bool connect();

public:
	static Redis* getInstance();
	static void destoryInstance();
	bool createData(std::string&&, std::string&&);
	std::string readData(std::string&);
	bool delData(std::string&&);
};

#endif // !REDIS_H
