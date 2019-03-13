#pragma once

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "util.h"
#include "config.h"
#include <thread>

int demo_stream(void);

int demo_stream_w_queue(void);

class obj_det_caffe
{
public:
	obj_det_caffe(void);
	void load_model(const char* graph_file, const char* weight_file);
	cv::Mat inference(cv::Mat & frame);

private:
	cv::dnn::Net model;
};
