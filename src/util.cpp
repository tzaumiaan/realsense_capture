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

void rs_frame_to_cv_rgb(rs2::video_frame & frame, cv::Mat &image_rgb)
{
	int w = frame.get_width();
	int h = frame.get_height();
	image_rgb = cv::Mat(cv::Size(w, h), CV_8UC3, (void*)frame.get_data(), cv::Mat::AUTO_STEP);
}

void visualize(rs2::video_frame &image, const char * const window_name, double frame_rate)
{
	cv::Mat image_rgb;
	rs_frame_to_cv_rgb(image, image_rgb);
	cv::Mat image_bgr;
	cv::cvtColor(image_rgb, image_bgr, cv::COLOR_RGB2BGR);
	std::stringstream text;
	text << "FPS: " << frame_rate;
	cv::putText(image_bgr, text.str(), cvPoint(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255, 255, 255), 1, CV_AA);
	cv::imshow(window_name, image_bgr);
}
void visualize(cv::Mat &image, const char * const window_name, double frame_rate)
{
	cv::Mat image_bgr;
	cv::cvtColor(image, image_bgr, cv::COLOR_RGB2BGR);
	std::stringstream text;
	text << "FPS: " << frame_rate;
	cv::putText(image_bgr, text.str(), cvPoint(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255, 255, 255), 1, CV_AA);
	cv::imshow(window_name, image_bgr);
}

void vis_bbox(cv::Mat & image_out, cv::Mat & image_in, cv::Mat & det_in)
{
	cv::Mat det_mat(det_in.size[2], det_in.size[3], CV_32F, det_in.ptr<float>());
	image_out = image_in.clone();

	float SCORE_THR = static_cast<float>(0.2);
	for (int i = 0; i < det_mat.rows; i++)
	{
		float score = det_mat.at<float>(i, 2);
		if (score > SCORE_THR)
		{
			int idx = static_cast<int>(det_mat.at<float>(i, 1));
			int x_min = static_cast<int>(det_mat.at<float>(i, 3) * image_in.cols);
			int y_min = static_cast<int>(det_mat.at<float>(i, 4) * image_in.rows);
			int x_max = static_cast<int>(det_mat.at<float>(i, 5) * image_in.cols);
			int y_max = static_cast<int>(det_mat.at<float>(i, 6) * image_in.rows);
			cv::Rect bbox(x_min, y_min, (x_max - x_min), (y_max - y_min));
			cv::rectangle(image_out, bbox, cv::Scalar(0, 255, 0), 2);
			std::ostringstream score_str;
			score_str << score;
			cv::String text = label_map[idx] + "(" + cv::String(score_str.str()) + ")";
			cv::putText(image_out, text, cv::Point(x_min, y_min),
				cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
		}
	}
}
