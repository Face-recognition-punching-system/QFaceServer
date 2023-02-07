#ifndef _include_opencv_rnet_h_
#define _include_opencv_rnet_h_

#include "face.h"
#include <opencv2/dnn.hpp>
/// <summary>
/// ������ṹ����ͨ���߽��Ļع�� NMS ��ȥ����Щ������������
/// ֻ�����ڸ�����ṹ��P-Net����ṹ�в��죬����һ��ȫ���Ӳ㣬
/// ���Ի�ȡ�ø��õ����Ƽ����������á�
/// </summary>
class RefineNetwork {
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
  RefineNetwork(const RefineNetwork::Config &config);
  ~RefineNetwork();

private:
  RefineNetwork(const RefineNetwork &rhs) = delete;
  RefineNetwork &operator=(const RefineNetwork &rhs) = delete;

public:
  std::vector<Face> run(const cv::Mat &img, const std::vector<Face> &faces);
};

#endif
