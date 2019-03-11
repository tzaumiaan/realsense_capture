#include "util.h"

using namespace std::chrono;

std::string get_time_str(void)
{
	auto time_point = system_clock::now().time_since_epoch();
	std::time_t time_full = (std::chrono::duration_cast<std::chrono::milliseconds>(time_point)).count();
	std::time_t time_s = time_full / 1000;
	std::time_t time_ms = time_full % 1000;
	std::tm time_in_tm;
	localtime_s(&time_in_tm, &time_s);
	std::stringstream buffer;
	buffer.fill('0');
	buffer << std::put_time(&time_in_tm, "%Y%m%d_%H%M%S") << "_" << std::setw(3) << time_ms;
	return buffer.str();
}

double get_frame_rate(std::time_t &prev_time_ms)
{
	auto time_point = system_clock::now().time_since_epoch();
	std::time_t cur_time_ms = (std::chrono::duration_cast<std::chrono::milliseconds>(time_point)).count();
	std::time_t diff = cur_time_ms - prev_time_ms;
	prev_time_ms = cur_time_ms;
	double frame_rate = 1000 / (double)diff;
	return frame_rate;
}

void visualize(rs2::video_frame &image, const char * const window_name, double frame_rate)
{
	int w = image.get_width();
	int h = image.get_height();
	cv::Mat image_rgb(cv::Size(w, h), CV_8UC3, (void*)image.get_data(), cv::Mat::AUTO_STEP);
	cv::Mat image_bgr;
	cv::cvtColor(image_rgb, image_bgr, cv::COLOR_RGB2BGR);
	std::stringstream text;
	text << "FPS: " << frame_rate;
	cv::putText(image_bgr, text.str(), cvPoint(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255, 255, 255), 1, CV_AA);
	cv::imshow(window_name, image_bgr);
}
