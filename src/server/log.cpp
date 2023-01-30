#include "log.h"

std::shared_ptr<Log> Log::_instance(new(std::nothrow) Log(), Log::destoryInstance);

void Log::destoryInstance(Log* instance) {
	delete instance;
}

Log::Log():_log(Poco::Logger::get("log")) {
	_log.information("log initialize");
}

Log::~Log() {
	_log.information("log uninitialize");
}

std::shared_ptr<Log> Log::getInstance() {
	return _instance;
}

void Log::fatal(const std::string& str) {
	_log.fatal(str);
}

void Log::critical(const std::string& str) {
	_log.critical(str);
}

void Log::error(const std::string& str) {
	_log.error(str);
}

void Log::warning(const std::string& str) {
	_log.warning(str);
}

void Log::notice(const std::string& str) {
	_log.notice(str);
}

void Log::information(const std::string& str) {
	_log.information(str);
}

void Log::debug(const std::string& str) {
	_log.debug(str);
}

void Log::trace(const std::string& str) {
	_log.trace(str);
}