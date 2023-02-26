#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>

namespace utils {
	std::string body(std::string&&, std::string&&);
	std::string body(std::string&&);
	std::string U2G(const std::string utf8);
	std::string G2U(const std::string gb2312);
	std::string base64Decode(const char*, int);
	std::string base64Encode(const unsigned char* Data, int);
	std::string Mat2Base64(const cv::Mat&, std::string);
	cv::Mat Base2Mat(std::string& base64_data);
}

#endif // !UTILS_H
