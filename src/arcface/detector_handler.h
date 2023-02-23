#ifndef DETECTOR_HANDLER_H
#define DETECTOR_HANDLER_H

#include <memory>

#include "detector.h"

class DetectorHandler {
private:
	static std::shared_ptr<DetectorHandler> _instance;
	MTCNNDetector* _detector;
	static void destoryInstance(DetectorHandler*);
	DetectorHandler();
	~DetectorHandler();

public:
	static std::shared_ptr<DetectorHandler> getInstance();
	cv::Mat detect(const cv::Mat&);
};

#endif // !DETECTOR_HANDLER_H
