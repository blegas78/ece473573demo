#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "turtlesim/Pose.h"

#include <sstream>

turtlesim::Pose latestMsg;
void cmdCallback(const turtlesim::Pose::ConstPtr& msg)
{
    latestMsg.x = msg->x;
    latestMsg.y = msg->y;
//  ROS_INFO_STREAM("I heard x:" << msg->linear.x << " theta:" << msg->angular.z);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");

  // NodeHandle is the main access point to communications with the ROS system.
  ros::NodeHandle n("~");

  /**
   * The advertise() function is how you tell ROS that you want to publish on a given topic name.
   */
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 100);

  ros::Publisher control_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 100);

  ros::Subscriber sub = n.subscribe("pose", 100, cmdCallback);

  std::string someString;
  n.param<std::string>("my_param", someString, "DEFAULT");	// Set default value if not provided
  ROS_INFO_STREAM("Got my_param: " << someString);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {
    // This is a message object. You stuff it with data, and then publish it.
    std_msgs::String msg;

    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    // The publish() function is how you send messages.
//    chatter_pub.publish(msg);

   ros::Time time = ros::Time::now();

    geometry_msgs::Twist command;
    command.linear.x = (1+sin( time.toSec() ))/(1+abs(latestMsg.x-5) + abs(latestMsg.y-5));
    command.angular.z = .1+.6*3.14159*cos( time.toSec() ) *(-12.5+(abs(latestMsg.x-5)*abs(latestMsg.y-5)))/12.5;

    control_pub.publish(command);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}

