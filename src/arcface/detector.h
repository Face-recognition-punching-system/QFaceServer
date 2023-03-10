#ifndef _include_opencv_mtcnn_detector_h_
#define _include_opencv_mtcnn_detector_h_

#include "face.h"
#include "onet.h"
#include "pnet.h"
#include "rnet.h"

class MTCNNDetector {
private:
    std::unique_ptr<ProposalNetwork> _pnet;
    std::unique_ptr<RefineNetwork> _rnet;
    std::unique_ptr<OutputNetwork> _onet;

public:
    MTCNNDetector();
    std::vector<Face> detect(const cv::Mat&, const float, const float, const float);
    cv::Mat faceAlign(const cv::Mat&, const Face&);
};

#endif
