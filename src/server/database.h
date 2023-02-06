#ifndef DATABASE_H
#define DATABASE_H

#include <Poco/JSON/Parser.h>
#include <mysql.h>
#include <mutex>
#include <memory>

class Database {
private:
	static Database* _instance;
	static std::recursive_mutex _mutex;
	MYSQL _connection;
	Database();
	~Database();
	bool connect();

public:
	static Database* getInstance();
	static void destroyInstance();
	std::string create(std::string&);
	std::string readOne(std::string&, std::initializer_list<std::string>);
	std::string read(std::string&, std::initializer_list<std::string>);
	std::string update(std::string&);
	std::string del(std::string&);
};

#endif // !DATABASE_H