#pragma once

#include <iostream>
#include <sstream>              // Stringstreams
#include <chrono>
#include <iomanip>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

// utility functions
std::string get_time_str(void);
double get_frame_rate(std::time_t &prev_time_ms);
void rs_frame_to_cv_rgb(rs2::video_frame &frame, cv::Mat &image_rgb);
void visualize(rs2::video_frame &image, const char *const window_name, double frame_rate);
void visualize(cv::Mat &image, const char * const window_name, double frame_rate);
void vis_bbox(cv::Mat & image_out, cv::Mat & image_in, cv::Mat & det_in);

// constants
const std::string label_map[] = { "background", "aeroplane", "bicycle", "bird", "boat",
	"bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
	"dog", "horse", "motorbike", "person", "pottedplant", "sheep",
	"sofa", "train", "tvmonitor" };