#include "database.h"
#include "logger.h"
#include "utils.h"
#include "../config/config.h"

// 饿汉模式
// Database* Database::_instance = new Database();

Database* Database::_instance(nullptr);

std::recursive_mutex Database::_mutex;

Database::Database() {
	mysql_init(&_connection);
	auto logger = Logger::getInstance();
	logger->information("database initialize\n");
	connect();
}

Database::~Database() {
	auto logger = Logger::getInstance();
	logger->information("database uninitialize\n");
	mysql_close(&_connection);
	mysql_library_end();
}

bool Database::connect() {
	auto logger = Logger::getInstance();
	if (mysql_real_connect(&_connection, host, username, password, database, 3306, nullptr, 0) == nullptr) {
		logger->information("database connect error");
		int code = static_cast<int>(mysql_errno(&_connection));
		std::string message = static_cast<std::string>(mysql_error(&_connection));
		logger->information(std::format("{} {}\n", message, code));
		return false;
	}
	else {
		char value = 1;
		mysql_options(&_connection, MYSQL_OPT_RECONNECT, &value);
		mysql_query(&_connection, "set names gb2312");
		logger->information("database connect success\n");
		return true;
	}
}

Database* Database::getInstance() {
	if (_instance == nullptr) {
		std::unique_lock<std::recursive_mutex> lock(_mutex);
		if (_instance == nullptr) {
			_instance = new(std::nothrow) Database();
		}
	}

	return _instance;
}

void Database::destroyInstance() {
	if (_instance != nullptr) {
		delete _instance;
		_instance = nullptr;
	}
}

std::string Database::create(std::string& query) {
	std::unique_lock<std::recursive_mutex> lock(_mutex);
	int status = static_cast<int>(mysql_real_query(&_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;
	if (status == 0) {
		int affect = static_cast<int>(mysql_affected_rows(&_connection));
		if (affect == 0) {
			return utils::body("error", "please check params are creect");
		}else if(affect == -1) {
			return utils::body("unknown error");
		}else {
			return utils::body("success");
		}
	}else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(&_connection));
		std::string message = static_cast<std::string>(mysql_error(&_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}

std::string Database::readOne(std::string& query, std::initializer_list<std::string> rest) {
	std::unique_lock<std::recursive_mutex> lock(_mutex);
	int status = static_cast<int>(mysql_real_query(&_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;
	if (status == 0) {
		MYSQL_RES* res = mysql_store_result(&_connection);
		int rows = static_cast<int>(mysql_num_rows(res));
		int fields = static_cast<int>(mysql_num_fields(res));
		std::cout << std::format("rows = {} fileds = {}\n", rows, fields) << std::endl;
		MYSQL_ROW line = mysql_fetch_row(res);
		if (rows){
			Poco::JSON::Object object;
			int i = 0;
			for (auto& r : rest) {
				object.set(r, std::format("{}", line[i]));
				i++;
			}

			std::stringstream ostream;
			object.stringify(ostream);
			mysql_free_result(res);
			return utils::G2U(ostream.str());
		}
		else{
			mysql_free_result(res);
			return "{}";
		}

	}else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(&_connection));
		std::string message = static_cast<std::string>(mysql_error(&_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}

std::string Database::read(std::string& query, std::initializer_list<std::string> rest) {
	std::unique_lock<std::recursive_mutex> lock(_mutex);
	int status = static_cast<int>(mysql_real_query(&_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;;
	if (status == 0) {
		MYSQL_RES* res = mysql_store_result(&_connection);
		int rows = static_cast<int>(mysql_num_rows(res)), fields = static_cast<int>(mysql_num_fields(res));
		std::cout << std::format("rows = {} fileds = {}\n", rows, fields) << std::endl;
		MYSQL_ROW line;
		if (rows) {
			Poco::JSON::Array array;
			while (line = mysql_fetch_row(res)) {
				Poco::JSON::Object object;
				int i = 0;
				for (auto& r : rest) {
					//object.set(r, std::format("{}", line[i] == NULL ? "" : line[i]));
					object.set(r, std::format("{}", line[i]));
					i++;
				}

				array.add(object);
			}

			std::stringstream ostream;
			array.stringify(ostream);
			mysql_free_result(res);
			return utils::G2U(ostream.str());
		}
		else {
			mysql_free_result(res);
			return "[]";
		}

	}else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(&_connection));
		std::string message = static_cast<std::string>(mysql_error(&_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}

std::string Database::update(std::string& query) {
	std::unique_lock<std::recursive_mutex> lock(_mutex);
	int status = static_cast<int>(mysql_real_query(&_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;
	if (status == 0) {
		int affect = static_cast<int>(mysql_affected_rows(&_connection));
		if (affect == 0) {
			return utils::body("error", "nothing need to update");
		}else if (affect == -1) {
			return utils::body("unknown error");
		}else {
			return utils::body("success");
		}
	}else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(&_connection));
		std::string message = static_cast<std::string>(mysql_error(&_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}

std::string Database::del(std::string& query) {
	std::unique_lock<std::recursive_mutex> lock(_mutex);
	int status = static_cast<int>(mysql_real_query(&_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;
	if (status == 0) {
		int affect = static_cast<int>(mysql_affected_rows(&_connection));
		if (affect == 0) {
			return utils::body("error", "nothing need to delete");
		}else if (affect == -1) {
			return utils::body("unknown error");
		}else {
			return utils::body("success");
		}
	}else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(&_connection));
		std::string message = static_cast<std::string>(mysql_error(&_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}

}

/*
try {
		std::stringstream sstream;
		Poco::MongoDB::Connection::SocketFactory socketFactory = Poco::MongoDB::Connection::SocketFactory();
		sstream << "mongodb://" << username << ":" << password << "@" << host << ":" << port << "/" << database;
		_connection.connect(sstream.str(), socketFactory);
		_state = true;
		auto logger = Logger::getInstance();
		logger->information("database initialize\n");
	}
	catch (const Poco::Exception& error) {
		std::printf("%s\n", error.displayText().c_str());
		throw error;
	}
*/