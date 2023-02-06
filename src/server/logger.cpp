#include "logger.h"

std::shared_ptr<Logger> Logger::_instance(new(std::nothrow) Logger(), Logger::destoryInstance);

void Logger::destoryInstance(Logger* instance) {
	delete instance;
}

Logger::Logger() {
	_logger = &Poco::Logger::root();
	_logger->information("log initialize\n");
}

Logger::~Logger() {
	_logger->information("log uninitialize\n");
	_logger = nullptr;
}

std::shared_ptr<Logger> Logger::getInstance() {
	return _instance;
}

void Logger::fatal(const std::string& str) {
	_logger->fatal(str);
}

void Logger::critical(const std::string& str) {
	_logger->critical(str);
}

void Logger::error(const std::string& str) {
	_logger->error(str);
}

void Logger::warning(const std::string& str) {
	_logger->warning(str);
}

void Logger::notice(const std::string& str) {
	_logger->notice(str);
}

void Logger::information(const std::string& str) {
	_logger->information(str);
}

void Logger::debug(const std::string& str) {
	_logger->debug(str);
}

void Logger::trace(const std::string& str) {
	_logger->trace(str);
}