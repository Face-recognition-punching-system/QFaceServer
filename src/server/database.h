#ifndef DATABASE_H
#define DATABASE_H

#include <Poco/MongoDB/MongoDB.h>
#include <Poco/MongoDB/Connection.h>
#include <Poco/MongoDB/Database.h>
#include <Poco/MongoDB/Cursor.h>
#include <Poco/MongoDB/Array.h>
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include <mutex>
#include <memory>

class Database {
private:
	static Database* _instance;
	static std::mutex _mutex;
	bool _state;
	Poco::MongoDB::Connection _connection;
	Database();
	~Database();

public:
	static Database* getInstance();
	static void destroyInstance();
	std::string createData();
	std::string readOneData();
	std::string readData();
	std::string updateData();
	std::string deleteData();
};

#endif // !DATABASE_H