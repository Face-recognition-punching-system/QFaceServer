#ifndef _include_opencv_onet_h_
#define _include_opencv_onet_h_

#include "face.h"
#include <opencv2/dnn.hpp>
/// <summary>
/// �ò�� R-Net ���ֶ���һ�����㣬���Դ���Ľ�������ϸ�����ú� R-Net ������һ����
/// ���Ǹò��������������˸���ļල��ͬʱ������� ������5 �� Landmark����꣩��
/// </summary>
class OutputNetwork {
public:
  struct Config {
  public:
    std::string protoText;
    std::string caffeModel;
    float threshold;
  };

private:
  cv::dnn::Net _net;
  float _threshold;

public:
  OutputNetwork(const OutputNetwork::Config &config);
  OutputNetwork();
 

private:
  OutputNetwork(const OutputNetwork &rhs) = delete;
  OutputNetwork &operator=(const OutputNetwork &rhs) = delete;

public:
  std::vector<Face> run(const cv::Mat &img, const std::vector<Face> &faces);
};

#endif
