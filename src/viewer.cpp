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
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */

//  Mat test = imread("test.jpg", CV_LOAD_IMAGE_COLOR);
//  imshow(test);
  ros::init(argc, argv, "VideoShow");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  	std::string topic; 
	if (n.getParam("viewerFeed", topic)){
		ROS_INFO("Got param: %s", topic.c_str());
	}else{
		ROS_ERROR("Failed to get param 'viewerFeed'");
	}
 	ros::Subscriber imSub = n.subscribe(topic, 1000, test);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}

