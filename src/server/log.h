#pragma once
#include <Poco/Logger.h>
#include <memory>
using  Poco::Logger;

class Log {
private:
	static std::shared_ptr<Log> _instance;
	static void destoryInstance(Log*);
	Poco::Logger& _log;
	Log();
	~Log();

public:
	static std::shared_ptr<Log> getInstance();
	void fatal(const std::string&);
	void critical(const std::string&);
	void error(const std::string&);
	void warning(const std::string&);
	void notice(const std::string&);
	void information(const std::string&);
	void debug(const std::string&);
	void trace(const std::string&);
};