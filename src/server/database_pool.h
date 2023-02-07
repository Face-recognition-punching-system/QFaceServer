#ifndef DATABASE_POOL_H
#define DATABASE_POOL_H

#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

#include "database.h"

class DatabasePool {
private:
	static DatabasePool* _instance;
	static std::mutex _mutex;
	const int _initSize;
	const int _maxSize;
	const int _maxIdleTime;
	const int _connectionTimeout;
	std::queue<Database*> _databaseQue;
	std::mutex _queueMutex;
	std::atomic_int _databaseCnt;
	std::condition_variable cv;
	DatabasePool();
	~DatabasePool();
	bool loadConfigFile();
	void produceConnectionTask();
	void scannerConnectionTask();

public:
	static DatabasePool* getInstance();
	static void destroyInstance();
	std::shared_ptr<Database> getDatabase();
};

#endif // !DATABASE_POOL_H
