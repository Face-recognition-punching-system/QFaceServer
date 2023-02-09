#include "database_pool.h"

DatabasePool* DatabasePool::_instance(nullptr);

std::mutex DatabasePool::_mutex;

DatabasePool::DatabasePool():_initSize(2), _maxSize(2), _maxIdleTime(60), _connectionTimeout(100){
	if (!loadConfigFile()) {
		return;
	}

	for (int i = 0; i < _initSize; ++i) {
		Database* p = new Database();
		p->connect();
		p->refreshAliveTime();
		_databaseQue.push(p);
		_databaseCnt++;
	}

	std::thread produce(std::bind(&DatabasePool::produceConnectionTask, this));
	produce.detach();

	std::thread scanner(std::bind(&DatabasePool::scannerConnectionTask, this));
	scanner.detach();
}

DatabasePool::~DatabasePool() {
	mysql_library_end();
}

bool DatabasePool::loadConfigFile() {
	return true;
	FILE* pf = fopen("mysql.ini", "r");
	if (pf == nullptr) {
		return false;
	}
	while (!feof(pf)) {
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		std::string str = line;
		double idx = str.find('=', 0);
		if (idx == -1) {//无效的配置项
			continue;
		}

		double endidx = str.find('\n', idx);
		std::string key = str.substr(0, idx);
		std::string value = str.substr(idx + 1, endidx - idx - 1);
		if (key == "ip") {
			std::string ip = value;
		}
		else if (key == "port") {
			int port = std::atoi(value.c_str());
		}
		else if (key == "dbname") {
			std::string dbname = value;
		}
		else if (key == "username") {
			std::string username = value;
		}
		else if (key == "password") {
			std::string password = value;
		}
		else if (key == "initSize") {
			int initSize = std::atoi(value.c_str());
		}
		else if (key == "maxSize") {
			int maxSize = std::atoi(value.c_str());
		}
		else if (key == "maxIdleTime") {
			int maxIdleTime = std::atoi(value.c_str());
		}
		else if (key == "connectionTimeout") {
			int connectionTimeout = std::atoi(value.c_str());
		}
	}
	return true;
}

void DatabasePool::produceConnectionTask() {
	for (;;) {
		std::unique_lock<std::mutex> lock(_queueMutex);
		while (!_databaseQue.empty()) {
			cv.wait(lock);
		}

		if (_databaseCnt < _maxSize) {
			Database* p = new Database();
			p->connect();
			p->refreshAliveTime();
			_databaseQue.push(p);
			_databaseCnt++;
		}

		cv.notify_all();
	}
}

void DatabasePool::scannerConnectionTask() {
	for (;;) {
		std::this_thread::sleep_for(std::chrono::seconds(_maxIdleTime));
		std::unique_lock<std::mutex> lock(_queueMutex);
		while (_databaseCnt > _initSize) {
			Database* p = _databaseQue.front();
			if (p->getAliveTime() >= (_maxIdleTime * 1000)) {
				_databaseQue.pop();
				_databaseCnt--;
				delete p;
			}
			else {
				break;
			}
		}
	}
}

DatabasePool* DatabasePool::getInstance() {
	if (_instance == nullptr) {
		std::unique_lock<std::mutex> lock(_mutex);
		if (_instance == nullptr) {
			_instance = new(std::nothrow) DatabasePool();
		}
	}

	return _instance;
}

void DatabasePool::destroyInstance() {
	if (_instance != nullptr) {
		delete _instance;
		_instance = nullptr;
	}
}

std::shared_ptr<Database> DatabasePool::getDatabase()
{
	std::unique_lock<std::mutex> lock(_queueMutex);
	while (_databaseQue.empty()) {
		if (std::cv_status::timeout == cv.wait_for(lock, std::chrono::milliseconds(_connectionTimeout))) {
			if (_databaseQue.empty()) {
				return nullptr;
			}
		}
	}

	std::shared_ptr<Database> sp(_databaseQue.front(),
		[&](Database* db) {
			std::unique_lock<std::mutex> lock(_queueMutex);
			db->refreshAliveTime();
			_databaseQue.push(db);
		});

	_databaseQue.pop();
	if (_databaseQue.empty()) {
		cv.notify_all();
	}

	return sp;
}