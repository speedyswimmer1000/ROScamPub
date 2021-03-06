#include "ros/ros.h"
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>     //make sure to include the relevant headerfiles
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/nonfree/nonfree.hpp>

using namespace cv;
using namespace cv_bridge;

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */

void test(const sensor_msgs::Image::ConstPtr& msg)
{
	ROS_INFO("IMG received");
	CvImagePtr cv_ptr;
	cv_ptr = toCvCopy(msg, "bgr8");
	Mat image(cv_ptr->image);
	namedWindow("Video", WINDOW_NORMAL);
	imshow("Video", image);
	waitKey(1); // Wait for 3 ms.
	Size s = image.size();

	cv::FastFeatureDetector detector( 50 ); 
	 // Sift seems much less crowded than Surf.
	cv::SurfDescriptorExtractor extractor;  
	// SURF descriptor calculation is much faster. 
	

	std::vector<cv::KeyPoint> raw_kps_keyframe;
	detector.detect(image, raw_kps_keyframe);
	cv::Mat descriptors_keyframe;
	extractor.compute(image, raw_kps_keyframe, descriptors_keyframe);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "VideoShow");

  ros::NodeHandle n;
  
  	std::string topic; 
	if (n.getParam("/videoFeed", topic)){
		ROS_INFO("Got param: %s", topic.c_str());
	}else{
		ROS_ERROR("Failed to get param 'videoFeed'");
	}
  	std::string topic2;
	if (n.getParam("videoFeed", topic2)){
		ROS_ERROR("Got param: %s", topic2.c_str());
	}else{
		ROS_ERROR("Failed to get param 'videoFeed'");
	}
 	ros::Subscriber imSub = n.subscribe(topic, 1000, test);

  ros::spin(); return 0; } 
