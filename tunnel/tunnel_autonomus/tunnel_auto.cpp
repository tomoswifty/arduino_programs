#include <boost/bind.hpp>
#include <ros/ros.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>
#include <std_srvs/Empty.h>
#include <sstream>
#include "std_msgs/String.h"
// for lidar
#include "sensor_msgs/LaserScan.h"
#define RAD2DEG(x) ((x)*180./M_PI)
int g_lidar_command;
void goalCommand(ros::Publisher goalPub, int controlSwitch, int command, int speed_l, int speed_r){
	std_msgs::String goalMsg;
	std::stringstream ss;
	ss << controlSwitch << "," << command << "," << speed_l  << "," << speed_r;
	goalMsg.data = ss.str();
	goalPub.publish(goalMsg);
}
// lidar callback function
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int right_iter = 481; // -45 degree
    int left_iter = 803;  // +45 degree
    bool obs_right = false; // <1.5m area detected in right area
    bool obs_left = false; // <1.5m area detected in left area
    bool forward = true;
    bool danger_area = false;
    for(int i = right_iter; i < left_iter+1; i++) {
        float range = scan->ranges[i];
        // check area <1.5m distance
        if (range < 1.5) {
			int middle_line = 642;
			// counted as right area
			if (right_iter <= i && i <= middle_line) {
				obs_right = true;
				forward = false;
			}
			// counted as left area
			else if (i > middle_line && i <= left_iter) {
				obs_left = true;
				forward = false;
			}
			// counted as danger area besides left and right area
			else {
				ROS_INFO (" DANGER AREAAAAAAAA!!!!!!");
				danger_area = true;
			}
		}
    }
	float left_degree = RAD2DEG(scan->angle_min + scan->angle_increment * left_iter);
	float left_range = scan->ranges[left_iter];
	
	float right_degree = RAD2DEG(scan->angle_min + scan->angle_increment * right_iter);
	float right_range = scan->ranges[right_iter];
	
	float inf = std::numeric_limits<float>::infinity();
	float gap_range = abs(left_range - right_range);
	
	if (obs_left == true || obs_right == true) {
		if (obs_left == true && obs_right == true) {
			ROS_INFO("OBS!! STOP");
			forward = false;
			g_lidar_command = 0;
		}
		else if (obs_left == false && obs_right == true) {
			ROS_INFO("OBS!! turn left ");
			// turn left command
			forward = false;
			g_lidar_command = 2;
		}
		else if (obs_left == true && obs_right == false) {
			// turn right command - L speed, R speed
			ROS_INFO("OBS!! turn right turn ");
			forward = false;
			g_lidar_command = 3;
		}
	}
	else {
		if (danger_area == false &&
		gap_range > 0.15 &&
		right_range != inf &&
		left_range != inf)
		{		
			if (right_range > left_range) {
				// forward right command - L speed, R speed
				ROS_INFO("going right forward ");
				forward = false;
				g_lidar_command = 7;
			}
			else if (left_range > right_range) {
				ROS_INFO("going left forward");
				// forward left command
				forward = false;
				g_lidar_command = 5;
			}
		}
	}
	
	if (forward == true){
		// going forward
		ROS_INFO("gong forward");
		g_lidar_command = 1;
	}
	
    ROS_INFO("gap range %f, Left: [%f, %f] :: Right: [%f, %f]", gap_range, left_degree, left_range, right_degree, right_range);
}
void lidarPub(const ros::TimerEvent&, ros::Publisher goalPub) {
	switch(g_lidar_command) {
		case 0:
			// stop
			goalCommand(goalPub, 8, 0, 0, 0);
			break;
		case 1:
			// going forward
			goalCommand(goalPub, 8, 1, 150, 150);
			break;
		case 2:
			// turn left
			goalCommand(goalPub, 8, 2, 130, 130);
			break;
		case 3:
			// turn right
			goalCommand(goalPub, 8, 3, 130, 130);
			break;
		case 5:
			// forward left command - L speed, R speed
			goalCommand(goalPub, 8, 5, 130, 150);
			break;
		case 7:
			// forward right command - L speed, R speed
			goalCommand(goalPub, 8, 7, 150, 130);
			break;
		
	}
}
int main(int argc, char **argv)
{
  char value[5];
  ros::init(argc, argv, "simple_move");
  ros::NodeHandle n;
  ros::Subscriber lidarSub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);
  ros::Publisher goalPub = n.advertise<std_msgs::String>("goal_data", 10);
  ros::Timer lidarTimer = n.createTimer(ros::Duration(0.1), boost::bind(lidarPub, _1, goalPub));
  ros::spin();
  return 0;
}