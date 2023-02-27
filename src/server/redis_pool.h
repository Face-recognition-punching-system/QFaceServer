#ifndef REDIS_H
#define REDIS_H

#include <memory>
#include <sw/redis++/redis++.h>

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
