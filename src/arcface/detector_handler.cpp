#include "detector_handler.h"

std::shared_ptr<DetectorHandler> DetectorHandler::_instance(new(std::nothrow) DetectorHandler(), destoryInstance);

void DetectorHandler::destoryInstance(DetectorHandler* instance) {
	delete instance;
}

DetectorHandler::DetectorHandler() {
	_detector = new MTCNNDetector();
}

DetectorHandler::~DetectorHandler() {
	delete _detector;
}

std::shared_ptr<DetectorHandler> DetectorHandler::getInstance() {
	return _instance;
}

cv::Mat DetectorHandler::detect(const cv::Mat& img) {
	int h = img.rows;
	int w = img.cols;
	float scale = 1.0;
	cv::Mat scimg(img);
	if (h > 108){
		scale = h / 108.0;
		h = h / scale;
		w = w / scale;
		cv::resize(img, scimg, cv::Size(w, h), 0, 0, 3);
	}

	cv::Mat face;
	std::vector<Face> faces = _detector->detect(scimg, 20.f, 0.5f, 0.709f);
	std::cout << faces.size() << std::endl;
	if (faces.size() == 1) {
		faces[0].faceScale(scale);
		face = _detector->faceAlign(img, faces[0]);
	}

	return face;
}