#include "facenet_handler.h"

std::shared_ptr<FacenetHandler> FacenetHandler::_instance(new(std::nothrow) FacenetHandler(), destoryInstance);

void FacenetHandler::destoryInstance(FacenetHandler* _instance) {
	delete _instance;
}

FacenetHandler::FacenetHandler() {
	_facenet = new Facenet();
	_facenet->datasetExtract();
}

FacenetHandler::~FacenetHandler() {
	delete _facenet;
}

std::shared_ptr<FacenetHandler> FacenetHandler::getInstance() {
	return _instance;
}

void FacenetHandler::datasetExtract() {
	std::unique_lock<std::mutex> lock(_mutex);
	_facenet->datasetExtract();
}

std::string FacenetHandler::faceRecognition(cv::Mat& face) {
	cv::Mat feat = _facenet->featureExtract(face);
	return _facenet->faceRecognition(feat, 1.06);
}