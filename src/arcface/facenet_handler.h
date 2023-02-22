#ifndef FACENET_HANDLER_H
#define FACENET_HANDLER_H

#include <mutex>
#include <memory>

#include "Facenet.h"

class FacenetHandler {
private:
	static std::shared_ptr<FacenetHandler> _instance;
	Facenet* _facenet;
	std::mutex _mutex;
	static void destoryInstance(FacenetHandler*);
	FacenetHandler();
	~FacenetHandler();

public:
	static std::shared_ptr<FacenetHandler> getInstance();
	void datasetExtract();
	std::string faceRecognition(cv::Mat&);
};

#endif // !FACENET_HANDLER_H
