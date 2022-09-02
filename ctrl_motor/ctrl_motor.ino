#include "ctrl_motor.h"

ros::NodeHandle nh;

void setup() {

  nh.initNode();

  nh.subscribe(motor);
  pinMode(PWM_M2A, OUTPUT);
  pinMode(PWM_M2B, OUTPUT);
  
  digitalWrite(PWM_M2A, LOW);       // MOTOR
  digitalWrite(PWM_M2B, LOW);
}

void loop() {

  unsigned long t = micros();

  if ((t - tTime[0]) >= (1000000 / MOTOR_FREQUENCY))
  {
    newtime_motor = micros();

    input = right_motor_ref;
    //right_motor_err = right_motor_ref - right_enc_w;
    //input = kp*right_motor_err;
    
    //motor_control = input;  // input va de 0 100 %
    motor_control = (int) (input*2.4 + 15); // input va de 0 100 %
  
    if (motor_control == 0) {
      digitalWrite(PWM_M2B, LOW);
      digitalWrite(PWM_M2A, LOW);
    }
    else if (motor_control > 0 and motor_control <= 255) {
      analogWrite(PWM_M2B, motor_control);
      digitalWrite(PWM_M2A, LOW);
    }
    else if (motor_control > 255) {
      digitalWrite(PWM_M2B, HIGH);
      digitalWrite(PWM_M2A, LOW);
    }
    else if (motor_control < 0 and motor_control >= -255) {
      digitalWrite(PWM_M2B, LOW);
      analogWrite(PWM_M2A, motor_control*(-1));
    }
    else if (motor_control < -255) {
      digitalWrite(PWM_M2B, LOW);
      digitalWrite(PWM_M2A, HIGH);
    }

    input = last_input;
    right_motor_err = last_right_motor_err;
    
    oldtime_motor = newtime_motor;
    tTime[0] = t;
  }

  nh.spinOnce();
}


void motor_cb(const std_msgs::Float32& motor_msg) {

  right_motor_ref = motor_msg.data;
}
