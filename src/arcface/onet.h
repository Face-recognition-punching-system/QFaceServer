#ifndef _include_opencv_onet_h_
#define _include_opencv_onet_h_

#include "face.h"
#include <opencv2/dnn.hpp>
/// <summary>
/// 该层比 R-Net 层又多了一层卷积层，所以处理的结果会更精细。作用和 R-Net 层作用一样。
/// 但是该层对人脸区域进行了更多的监督，同时还会输出 人脸的5 个 Landmark（界标）。
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
