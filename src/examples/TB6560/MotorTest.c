// to build:
//  xcc main.xc -O0 -target=XCORE-200-EXPLORER  -o testpins.xe  
// or, try with a custom XN file stored locally: 
//  xcc main.xc my_board.xn -O0 -o testpins.xe
// to run:
//  xrun --xscope testpins.xe
// for offline, detailed timing analysis (no chip needed):
//  xsim testpins.xe  --vcd-tracing "-o trace.vcd -tile tile[0] -ports -ports-detailed -clock-blocks -instructions -cores"


#include <xs1.h>
#include <syscall.h>
#include <timer.h>
#include <print.h>
#include <platform.h>
#include <stdint.h>
#include <xclib.h>


port T0_P1A  = XS1_PORT_1A; // X0D00 DIRPIN
port T0_P1B  = XS1_PORT_1B; // X0D01 STEPPIN

/* Example sketch to control a stepper motor with TB6560 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */

// Define stepper motor connections and steps per revolution:

#define stepsPerRevolution 1600

int main() {
  
  // Set the spinning direction clockwise:
  //digitalWrite(dirPin, HIGH);
  T0_P1A <: 1;

  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    //digitalWrite(stepPin, HIGH);
    T0_P1B <: 1;
    delayMicroseconds(2000);
    //digitalWrite(stepPin, LOW);
    T0_P1B <: 0;
    delayMicroseconds(2000);
  }

  delay_milliseconds(1000);

  // Set the spinning direction counterclockwise:
  //digitalWrite(dirPin, LOW);
  T0_P1A <: 0;

  // Spin the stepper motor 1 revolution quickly:
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    // These four lines result in 1 step:
    //digitalWrite(stepPin, HIGH);
    T0_P1B <: 1;
    delayMicroseconds(1000);
    //digitalWrite(stepPin, LOW);
    T0_P1B <: 0;
    delayMicroseconds(1000);
    
  }

  delay_milliseconds(1000);

  // Set the spinning direction clockwise:
  //digitalWrite(dirPin, HIGH);
  T0_P1A <: 1;

  // Spin the stepper motor 5 revolutions fast:
  for (int i = 0; i < 5 * stepsPerRevolution; i++) {
  // These four lines result in 1 step:
    //digitalWrite(stepPin, HIGH);
    T0_P1B <: 1;
    delayMicroseconds(500);
    //digitalWrite(stepPin, LOW);
    T0_P1B <: 0;
    delayMicroseconds(500);
  }

  delay_milliseconds(1000);

  // Set the spinning direction counterclockwise:
  //digitalWrite(dirPin, LOW);


  // Spin the stepper motor 5 revolutions fast:
  for (int i = 0; i < 5 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    //digitalWrite(stepPin, HIGH);
    T0_P1B <: 1;
    delayMicroseconds(500);
    //digitalWrite(stepPin, LOW);
    T0_P1B <: 0;
    delayMicroseconds(500);
  }

  delay_milliseconds(1000);

  
}

