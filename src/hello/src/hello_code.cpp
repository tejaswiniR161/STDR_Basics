#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Bool.h> 
#include <sensor_msgs/LaserScan.h>
geometry_msgs::Twist directions;
int rotatecount=0;
int forward=0;
int popeye=0;
void laserCallback(const sensor_msgs::LaserScan& laser_scan) 
{
    std_msgs::String msg;
    std::stringstream ss;
	int f=0;
	int infcount=0;
	rotatecount=0;
	popeye=0;
	for(int k=85;k<95;k++)
	{
		if(laser_scan.ranges[k]<1)
		popeye=1;

	}
	
	if(popeye==1)
	{
	for(int i=0;i<270;i++)
	{

		if(laser_scan.ranges[i]>laser_scan.range_max && infcount<10)
			{
				infcount++;
			}
		else if(infcount>=10)
			continue;
		else
			{

				infcount=0;				
				
			}
	rotatecount++;
		ss<<laser_scan.ranges[i]<<"  ";
		//ss+="  ";
		if(rotatecount>259)
		rotatecount=135;
	}
	}
	
	ss<<"infcount : "<<infcount <<" rotatecount : "<<rotatecount;
    ss << "range_min  :  "<<laser_scan.range_min<<"  range_max :  "<<laser_scan.range_max;
    msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "helloexe");
  ros::NodeHandle n;
  std_msgs::String msg;

std::stringstream somestring;
somestring<<"/"<<argv[argc-1]<<"/cmd_vel";

std::stringstream somestringsub;
somestringsub<<argv[argc-1]<<"/laser_1";

  std::stringstream ss;
	//for(int y=0;y<argc;y++)
	//{
		ss<<" arg ==   "<<argv[argc-1];
	//}
	msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());
  //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  ros::Publisher twist_mover = n.advertise<geometry_msgs::Twist>(somestring.str(), 1);
  ros::Subscriber lidar_subscriber1 = n.subscribe(somestringsub.str(), 1, laserCallback);
  //ros::Publisher twist_mover = n.advertise<geometry_msgs::Twist>("/robot0/cmd_vel", 1);
  //ros::Subscriber lidar_subscriber1 = n.subscribe("robot0/laser_1", 1, laserCallback);
  //ros::Subscriber lidar_subscriber0 = n.subscribe("robot0/laser_0", 1, laserCallback);
  ros::Rate loop_rate(1000);
  geometry_msgs::Twist directions;
  directions.linear.x=1;
  directions.linear.y=0;
  directions.linear.z=0;
  directions.angular.x=0;
  directions.angular.y=0;
  directions.angular.z=0;

  int count = 0;
  while (ros::ok())
  {
	if(popeye==1)
	{
	for(int j=0;j<rotatecount;j++)
	{
		directions.angular.z=1;
		//directions.angular.x=1;
		directions.linear.x=0;
		twist_mover.publish(directions);
		/*if(j>=rotatecount-1)
		{
			directions.linear.x=1;
			twist_mover.publish(directions);
		}*/
	}
	}	
    //twist_mover.publish(directions);
	else
	{
		directions.linear.x=1;
		twist_mover.publish(directions);
	}
    //directions.linear.x=1;
	//twist_mover.publish(directions);
    ros::spinOnce();
    loop_rate.sleep();
    
  }
  return 0;
}
