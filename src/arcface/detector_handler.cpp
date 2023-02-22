#include "detector_handler.h"

std::unique_ptr<DetectorHandler> DetectorHandler::_instance(new(std::nothrow) DetectorHandler(), destoryInstance);

void DetectorHandler::destoryInstance(DetectorHandler* instance) {
	delete instance;
}

DetectorHandler::DetectorHandler() {
	_detector = new MTCNNDetector();
}

DetectorHandler::~DetectorHandler() {
	delete _detector;
}

std::unique_ptr<DetectorHandler> DetectorHandler::getInstance() {
	return _instance;
}

std::vector<cv::Mat> DetectorHandler::detect(const cv::Mat& img) {
	int h = img.rows;
	int w = img.cols;
	float scale = 1.0;
	cv::Mat scimg(img);
	if (h > 108)
	{
		scale = h / 108.0;
		h = h / scale;
		w = w / scale;
		cv::resize(img, scimg, cv::Size(w, h), 0, 0, 3);
	}

	std::vector<Face> faces = _detector->detect(img, 20.f, 0.5f, 0.709f);
	for (int i = 0; i < faces.size(); i++)
	{
		//�Ŵ����
		faces[i].faceScale(scale);
	}

	std::vector<Face> def{ faces[i] };
	return _detector->faceAlign(img, def);
}