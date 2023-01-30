#include "database.h"
#include "../config/config.h"
#include "log.h"

// 饿汉模式
// Database* Database::_instance = new Database();

Database* Database::_instance(nullptr);

std::mutex Database::_mutex;

Database::Database() {
	_state = false;
	try{
		std::stringstream sstream;
		Poco::MongoDB::Connection::SocketFactory socketFactory = Poco::MongoDB::Connection::SocketFactory();
		sstream << "mongodb://" << username << ":" << password << "@" << host << ":" << port << "/" << database;
		_connection.connect(sstream.str(), socketFactory);
		_state = true;
		std::printf("%s\n", "数据库连接成功");
	}catch (const std::exception& error){
		std::printf("%s\n", "数据库连接失败");
		throw error;
	}
}

Database::~Database() {
	if (_state) {
		_connection.disconnect();
	}

}

Database* Database::getInstance() {
	// 懒汉模式
	// 双检锁
	if (_instance == nullptr) {
		// 加锁
		std::unique_lock<std::mutex> lock(_mutex);
		if (_instance == nullptr) {
			// 内存不足时置 nullptr
			_instance = new(std::nothrow) Database();
		}
	}

	std::printf("%s\n", "获取实例");
	return _instance;
}

void Database::destroyInstance() {
	if (_instance != nullptr) {
		std::printf("%s\n", "销毁实例");
		delete _instance;
	}
}

std::string Database::createData() {
	return "";
}

std::string Database::readOneData() {
	return "";
}

std::string Database::readData() {
	return "";
}

std::string Database::updateData() {
	return "";
}

std::string Database::deleteData() {
	return "";
}