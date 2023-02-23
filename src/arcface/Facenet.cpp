#include "Facenet.h"
#include "utils.h"

Facenet::Facenet()
{
	_net = cv::dnn::readNetFromTensorflow("model/facenet.pb");
	if (_net.empty()) {
		throw std::invalid_argument("facenet loading error");
	}
}

void Facenet::datasetExtract()
{
	_feat.clear();
	_vimgname.clear();
	std::vector<std::string> imgspath;
	cv::glob("img/*.jpg", imgspath);
	for (int i = 0; i < imgspath.size(); i++)
	{
		cv::Mat img = cv::imread(imgspath[i]);
		if (img.empty()) {
			continue;
		}

		cv::Mat feat = featureExtract(img);
		_feat.push_back(feat);
		_vimgname.push_back(imgspath[i]);
	}
}


cv::Mat Facenet::featureExtract(const cv::Mat& img)
{
	cv::Mat fimg = convertImg(img);
	cv::Mat inputBlob = cv::dnn::blobFromImage(fimg, IMG_INV_STDDEV, cv::Size(160, 160),
		cv::Scalar(IMG_MEAN, IMG_MEAN, IMG_MEAN), false);
	_net.setInput(inputBlob);
	std::vector<cv::Mat> outputBlobs;
	_net.forward(outputBlobs);
	return outputBlobs[0].clone();
}

double Facenet::getSimilarity(const cv::Mat& feat1, const cv::Mat& feat2)
{
	return cv::norm(feat1 - feat2);
}

std::string Facenet::faceRecognition(cv::Mat& feat, double threshold)
{
	int id = -1;
	double mins = 0.;
	for (int i = 0; i < _feat.size(); i++)
	{
		double simx = getSimilarity(_feat[i], feat);
		if (simx < mins || id == -1)
		{
			id = i;
			mins = simx;
		}
	}
	if (mins > threshold || id==-1)return "none";
	if (_nametolabel.count(_vimgname[id]) > 0)
		return _nametolabel[_vimgname[id]];

	return "none";
}