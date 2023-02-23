#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

class Facenet
{
public:
	Facenet();
	cv::Mat featureExtract(const cv::Mat& img);
	double getSimilarity(const cv::Mat& feat1, const cv::Mat& feat2);
	void datasetExtract();
	std::string faceRecognition(cv::Mat& feat, double threshold);
private:
	cv::dnn::Net _net;
	std::vector<cv::Mat> _feat;
	std::vector<std::string> _vimgname;
	std::map<std::string, std::string> _nametolabel;
};

