#include <Poco/JSON/Parser.h>
#include <mutex>
#include <memory>

#include "database.h"
#include "logger.h"
#include "utils.h"
#include "../config/config.h"

Database::Database() {
	_connection = mysql_init(nullptr);
	auto logger = Logger::getInstance();
	logger->information("database initialize\n");
}

Database::~Database() {
	if (_connection != nullptr) {
		mysql_close(_connection);
	}

	auto logger = Logger::getInstance();
	logger->information("database uninitialize");
}

bool Database::connect() {
	MYSQL* p = mysql_real_connect(_connection, host, username, password, database, 3306, nullptr, 0);
	char value = 1;
	mysql_options(_connection, MYSQL_OPT_RECONNECT, &value);
	mysql_query(_connection, "set names gb2312");
	auto logger = Logger::getInstance();
	if (p == nullptr) {
		logger->information("database connect error");
		int code = static_cast<int>(mysql_errno(_connection));
		std::string message = static_cast<std::string>(mysql_error(_connection));
		logger->information(std::format("{} {}\n", message, code));
		return false;
	} else {
		logger->information("database connect success\n");
		return true;
	}
}

std::string Database::create(std::string& query) {
	int status = static_cast<int>(mysql_real_query(_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;
	if (status == 0) {
		int affect = static_cast<int>(mysql_affected_rows(_connection));
		if (affect == 0) {
			return utils::body("error", "please check params are exist");
		} else if (affect == -1) {
			return utils::body("unknown error");
		} else {
			return utils::body("success");
		}
	} else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(_connection));
		std::string message = static_cast<std::string>(mysql_error(_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}

std::string Database::readOne(std::string& query, std::initializer_list<std::string> rest) {
	int status = static_cast<int>(mysql_real_query(_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;
	if (status == 0) {
		MYSQL_RES* res = mysql_store_result(_connection);
		int rows = static_cast<int>(mysql_num_rows(res));
		int fields = static_cast<int>(mysql_num_fields(res));
		std::cout << std::format("rows = {} fileds = {}\n", rows, fields) << std::endl;
		MYSQL_ROW line = mysql_fetch_row(res);
		if (rows) {
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
		} else {
			mysql_free_result(res);
			return "{}";
		}
	} else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(_connection));
		std::string message = static_cast<std::string>(mysql_error(_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}

std::string Database::read(std::string& query, std::initializer_list<std::string> rest) {
	int status = static_cast<int>(mysql_real_query(_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;;
	if (status == 0) {
		MYSQL_RES* res = mysql_store_result(_connection);
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
		} else {
			mysql_free_result(res);
			return "[]";
		}
	} else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(_connection));
		std::string message = static_cast<std::string>(mysql_error(_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}

std::string Database::update(std::string& query) {
	int status = static_cast<int>(mysql_real_query(_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;
	if (status == 0) {
		int affect = static_cast<int>(mysql_affected_rows(_connection));
		if (affect == 0) {
			return utils::body("error", "nothing need to update");
		} else if (affect == -1) {
			return utils::body("unknown error");
		} else {
			return utils::body("success");
		}
	} else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(_connection));
		std::string message = static_cast<std::string>(mysql_error(_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}

std::string Database::del(std::string& query) {
	int status = static_cast<int>(mysql_real_query(_connection, query.c_str(),
		static_cast<unsigned int>(query.size())));
	std::cout << std::format("query = {}\n", query) << std::endl;
	if (status == 0) {
		int affect = static_cast<int>(mysql_affected_rows(_connection));
		if (affect == 0) {
			return utils::body("error", "nothing need to delete");
		} else if (affect == -1) {
			return utils::body("unknown error");
		} else {
			return utils::body("success");
		}
	} else {
		auto logger = Logger::getInstance();
		int code = static_cast<int>(mysql_errno(_connection));
		std::string message = static_cast<std::string>(mysql_error(_connection));
		logger->information(std::format("{} {}\n", message, code));
		return utils::body("unknown error");
	}
}