#ifndef DATABASE_H
#define DATABASE_H

#include <Poco/JSON/Parser.h>
#include <mysql.h>
#include <mutex>
#include <memory>

class Database {
private:
	MYSQL* _connection;
	clock_t _alivetime;

public:
	Database();
	~Database();
	bool connect();
	std::string create(std::string&);
	std::string readOne(std::string&, std::initializer_list<std::string>);
	std::string read(std::string&, std::initializer_list<std::string>);
	std::string update(std::string&);
	std::string del(std::string&);
	inline clock_t getAliveTime() const {
		return clock() - _alivetime;
	}
	inline void refreshAliveTime() {
		_alivetime = clock();
	}
};

#endif // !DATABASE_H