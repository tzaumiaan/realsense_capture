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
void visualize(rs2::video_frame &image, const char *const window_name, double frame_rate);