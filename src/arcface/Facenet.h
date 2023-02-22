#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class Facenet
{
public:
	Facenet();
	cv::Mat featureExtract(const cv::Mat& img);
	double getSimilarity(const cv::Mat& feat1, const cv::Mat& feat2);
	void datasetExtract();
	string faceRecognition(cv::Mat& feat, double threshold);
private:
	cv::dnn::Net _net;
	vector<cv::Mat> _feat;
	vector<string> _vimgname;
	map<string, string> _nametolabel;
};

