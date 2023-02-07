#ifndef _include_opencv_pnet_h_
#define _include_opencv_pnet_h_

#include "face.h"
#include "utils.h"
#include <opencv2/dnn.hpp>
/// <summary>
/// 该网络结构主要获得了人脸区域的候选窗口和边界框的回归向量，
/// 并用该边界框做回归，对候选窗口进行校准，然后通过非极大值抑制 (NMS) 
/// 来合并高度重叠的候选框。
/// </summary>
class ProposalNetwork {
public:
  struct Config {
  public:
    std::string protoText;
    std::string caffeModel;
    float threshold;
    float nmsThreshold;
  };

private:
  cv::dnn::Net _net;
  float _threshold;

private:
  std::vector<Face> buildFaces(const cv::Mat &scores,
                               const cv::Mat &regressions,
                               const float scaleFactor, const float threshold);

public:
  ProposalNetwork(const ProposalNetwork::Config &config);
  ~ProposalNetwork();

private:
  ProposalNetwork(const ProposalNetwork &rhs) = delete;
  ProposalNetwork &operator=(const ProposalNetwork &rhs) = delete;

public:
  std::vector<Face> run(const cv::Mat &img, const float minFaceSize, const float maxFacescale,
                        const float scaleFactor);
};

#endif
