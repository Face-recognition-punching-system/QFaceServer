#pragma once
#include <Poco/Logger.h>
#include <memory>

class Logger {
private:
	static std::shared_ptr<Logger> _instance;
	static void destoryInstance(Logger*);
	Poco::Logger* _logger;
	Logger();
	~Logger();

public:
	static std::shared_ptr<Logger> getInstance();
	void fatal(const std::string&);
	void critical(const std::string&);
	void error(const std::string&);
	void warning(const std::string&);
	void notice(const std::string&);
	void information(const std::string&);
	void debug(const std::string&);
	void trace(const std::string&);
};