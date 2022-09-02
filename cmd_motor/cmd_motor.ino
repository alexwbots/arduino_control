#include "pid_motor.h"

ros::NodeHandle nh;

void setup() {

  nh.initNode();
  
  nh.advertise(encoder);
  nh.subscribe(motor);

  pinMode(ENC_IN_RIGHT_A , INPUT_PULLUP);
  pinMode(ENC_IN_RIGHT_B , INPUT);

  pinMode(PWM_M2A, OUTPUT);
  pinMode(PWM_M2B, OUTPUT);

  digitalWrite(PWM_M2A, LOW);       // MOTOR
  digitalWrite(PWM_M2B, LOW);

  attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A), right_wheel_tick, RISING);
}

void loop() {

  unsigned long t = micros();

  if ((t - tTime[0]) >= (1000000 / ENCODER_FREQUENCY))
  {
    right_newtime_encoder = micros();

    right_delta_t_enc = 1.0/ENCODER_FREQUENCY;
    right_temp = (right_wheel_tick_count - last_right_wheel_tick_count)/right_delta_t_enc;
    right_enc_w = right_temp*(2.0*PI)/(7.0*60.0);
    right_w_msg.data = right_enc_w;  
    encoder.publish(&right_w_msg);

    // Resolucion de 1.496 rad/s a una frecuencia de 100 Hz
    // Resolucion de 0.1496 rad/s a una frecuencia de 10 Hz

    last_right_wheel_tick_count = right_wheel_tick_count;
    
    right_oldtime_encoder = right_newtime_encoder;
    tTime[0] = t;
  }

  if ((t - tTime[1]) >= (1000000 / MOTOR_FREQUENCY))
  {
    right_newtime_motor = micros();

    right_motor_err2 = right_motor_err1;
    right_motor_err1 = right_motor_err0;
    right_motor_err0 = right_motor_ref - right_enc_w;

    right_input = right_input + right_A0*right_motor_err0 + right_A1*right_motor_err1 + right_A2*right_motor_err2;
    
    motor_control = (int) (right_input*2.45 + 10); // input va de 0 100 %
  
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
    
    right_oldtime_motor = right_newtime_motor;
    tTime[1] = t;
  }

  nh.spinOnce();
}


// Increment the number of ticks
void right_wheel_tick() {
   
  // Read the value for the encoder for the right wheel
  int val = digitalRead(ENC_IN_RIGHT_B);
 
  if(val == LOW) {
    right_wheel_tick_count++; 
  }
  else {
    right_wheel_tick_count--;
  }
}


void motor_cb(const std_msgs::Float32& motor_msg) {

  right_motor_ref = motor_msg.data;
}
