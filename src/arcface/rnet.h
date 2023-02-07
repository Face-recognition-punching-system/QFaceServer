#ifndef _include_opencv_rnet_h_
#define _include_opencv_rnet_h_

#include "face.h"
#include <opencv2/dnn.hpp>
/// <summary>
/// 该网络结构还是通过边界框的回归和 NMS 来去掉那些假正例的区域。
/// 只是由于该网络结构和P-Net网络结构有差异，多了一个全连接层，
/// 所以会取得更好的抑制假正例的作用。
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
