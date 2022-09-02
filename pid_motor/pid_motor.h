#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>

#define ENCODER_FREQUENCY                 10   //hz
#define MOTOR_FREQUENCY                 1000   //hz

#define PWM_M2A    10
#define PWM_M2B    11

#define RIGHT_KP   20.00
#define RIGHT_KD    5.00
#define RIGHT_KI    0.01

#define ENC_IN_RIGHT_A 21
#define ENC_IN_RIGHT_B 20

float right_A0 = RIGHT_KP + RIGHT_KI*(1.0/MOTOR_FREQUENCY) + RIGHT_KD/(1.0/MOTOR_FREQUENCY);
float right_A1 = -RIGHT_KP - 2.0*RIGHT_KD/(1.0/MOTOR_FREQUENCY);
float right_A2 = RIGHT_KD/(1.0/MOTOR_FREQUENCY);

unsigned long  right_oldtime_encoder = 0;
unsigned long  right_newtime_encoder;

unsigned long  right_oldtime_motor = 0;
unsigned long  right_newtime_motor;

int motor_control = 0;

float right_input = 0.0;
float right_temp = 0.0;
float right_enc_w = 0.0;
float right_motor_ref = 0.0;
float right_motor_err2 = 0.0;
float right_motor_err1 = 0.0;
float right_motor_err0 = 0.0;
float right_delta_t_enc = 0.0;
float right_delta_t_mtr = 0.0;
float last_right_enc_w = 0.0;
float last_right_motor_err = 0.0;
volatile int right_wheel_tick_count = 0.0;
volatile int last_right_wheel_tick_count = 0.0;

unsigned long tTime[4];


/********* Publishers *********/

std_msgs::Float32 right_w_msg;
ros::Publisher encoder("encoder", &right_w_msg);


/********* Subscribers *********/

void motor_cb(const std_msgs::Float32& motor_msg);
ros::Subscriber<std_msgs::Float32> motor("motor", motor_cb);
