#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>

#define MOTOR_FREQUENCY                 1000   //hz

#define PWM_M2A  10
#define PWM_M2B  11

unsigned long  oldtime_motor = 0;
unsigned long  newtime_motor;

int motor_control = 0;

float input = 0;
float right_motor_err = 0;
float right_enc_w = 0;

float last_input = 0;
float last_right_motor_err = 0;

unsigned long tTime[4];


/********* Subscribers *********/

void motor_cb(const std_msgs::Float32& motor_msg);
ros::Subscriber<std_msgs::Float32> motor("motor", motor_cb);
