#include "utils.h"

std::string utils::body(std::string&& message, std::string&& detail) {
	Poco::JSON::Object object;
	object.set("message", message);
	object.set("detial", detail);
	std::stringstream ostream;
	object.stringify(ostream);
	return ostream.str();
}

std::string utils::body(std::string&& message) {
	Poco::JSON::Object object;
	object.set("message", message);
	std::stringstream ostream;
	object.stringify(ostream);
	return ostream.str();
}

std::string utils::U2G(const std::string utf8)
{
	double len = static_cast<double>(MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0));
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

std::string utils::G2U(const std::string gb2312)
{
	double len = static_cast<double>(MultiByteToWideChar(CP_ACP, 0, gb2312.c_str(), -1, NULL, 0));
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}