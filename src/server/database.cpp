#include "database.h"
#include "../config/config.h"
#include "log.h"

// ����ģʽ
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
		std::printf("%s\n", "���ݿ����ӳɹ�");
	}catch (const std::exception& error){
		std::printf("%s\n", "���ݿ�����ʧ��");
		throw error;
	}
}

Database::~Database() {
	if (_state) {
		_connection.disconnect();
	}

}

Database* Database::getInstance() {
	// ����ģʽ
	// ˫����
	if (_instance == nullptr) {
		// ����
		std::unique_lock<std::mutex> lock(_mutex);
		if (_instance == nullptr) {
			// �ڴ治��ʱ�� nullptr
			_instance = new(std::nothrow) Database();
		}
	}

	std::printf("%s\n", "��ȡʵ��");
	return _instance;
}

void Database::destroyInstance() {
	if (_instance != nullptr) {
		std::printf("%s\n", "����ʵ��");
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