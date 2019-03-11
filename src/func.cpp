#include "func.h"
#include "util.h"
#include "config.h"

#include <thread>

int demo_stream(void)
try {
	rs2::context ctx;
	auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
	if (list.size() == 0)
		throw std::runtime_error("No device detected. Is it plugged in?");
	//rs2::device dev = list.front();

	rs2::pipeline pipe;
	pipe.start();

	// Capture 30 frames to give autoexposure, etc. a chance to settle
	for (auto i = 0; i < 30; ++i) pipe.wait_for_frames();

	const auto window_name = "Display Image";
	cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
	std::time_t prev_time_ms = 0;
	while (cv::waitKey(1) < 0 && cvGetWindowHandle(window_name))
	{
		double frame_rate = get_frame_rate(prev_time_ms);
		rs2::frameset frames = pipe.wait_for_frames();
		rs2::video_frame image = frames.get_color_frame().as<rs2::video_frame>();
		visualize(image, window_name, frame_rate);
	}

	return EXIT_SUCCESS;
}
catch (const rs2::error &e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}

int demo_stream_w_queue(void)
try {
	rs2::pipeline pipe;
	pipe.start();

	const auto CAPACITY = 5; // allow max latency of 5 frames
	rs2::frame_queue queue(CAPACITY);
	std::thread t([&]() {
		// declaration for window handler and timer
		const auto window_name = "Display Image";
		cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
		std::time_t prev_time_ms = 0;
		while(cv::waitKey(1) < 0 && cvGetWindowHandle(window_name))
		{
			rs2::frame frame;
			if (queue.poll_for_frame(&frame))
			{
				auto image = rs2::video_frame(frame);
				std::cout << "dequeue" << get_time_str() << std::endl;
				// Do processing on the frame
				double frame_rate = get_frame_rate(prev_time_ms);
				visualize(image, window_name, frame_rate);
			}
		}
	});
	t.detach();

	while (true)
	{
		rs2::frameset frames = pipe.wait_for_frames();
		queue.enqueue(frames.get_color_frame());
		std::cout << "+++ enqueue" << get_time_str() << std::endl;
	}
}
catch (const rs2::error &e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}