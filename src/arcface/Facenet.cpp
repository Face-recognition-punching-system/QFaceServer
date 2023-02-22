#include "Facenet.h"
#include "utils.h"
Facenet::Facenet()
{
	_net = cv::dnn::readNetFromTensorflow("model/facenet.pb");
	if (_net.empty()) {
		cout << "facenet����ʧ��" << endl;
		throw std::invalid_argument("facenet loading error");
	}
}

void Facenet::datasetExtract()
{
	//��ȡ�����ͼƬ����
	_feat.clear();
	_vimgname.clear();
	vector<string> imgspath;
	cv::glob("img/*.jpg", imgspath);
	for (int i = 0; i < imgspath.size(); i++)
	{
		int st = 0;
		for (int j = 0; j < imgspath[i].size(); j++)
		{
			if (imgspath[i][j] == '\\')imgspath[i][j] = '/';
			if (imgspath[i][j] == '/')st = j + 1;
		}

		cv::Mat img = cv::imread(imgspath[i]);
		if (img.empty())continue;
		cv::Mat feat = featureExtract(img);
		_feat.push_back(feat);
		_vimgname.push_back(imgspath[i]);
	}
}


cv::Mat Facenet::featureExtract(const cv::Mat& img)
{
	
	//ͼ��ת���㣬����׼��
	cv::Mat fimg = convertImg(img);

	//img = imgStandardization(img);
	//cv::Mat inputBlob =	cv::dnn::blobFromImage(img, 1.0, cv::Size(160,160));
	cv::Mat inputBlob = cv::dnn::blobFromImage(fimg, IMG_INV_STDDEV, cv::Size(160, 160),
		cv::Scalar(IMG_MEAN, IMG_MEAN, IMG_MEAN), false);

	//����ģ������
	_net.setInput(inputBlob);
	//ǰ�򴫲�
	vector<cv::Mat> outputBlobs;
	_net.forward(outputBlobs);
	return outputBlobs[0].clone();
}

double Facenet::getSimilarity(const cv::Mat& feat1, const cv::Mat& feat2)
{
	//ŷʽ����
	return cv::norm(feat1 - feat2);
}
//Ѱ�����ݿ��������Ƶ�����
string Facenet::faceRecognition(cv::Mat& feat, double threshold)
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