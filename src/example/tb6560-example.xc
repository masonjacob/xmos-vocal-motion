/* Example sketch to control a stepper motor with TB6560 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */
// to build:
//  xcc tb6560-example.xc -O0 -target=XU316-1024-QF60A-C24  -o test.xe  
// or, try with a custom XN file stored locally: 
//  xcc main.xc my_board.xn -O0 -o test.xe ***my_board.xn not currently configured
// to run:
//  xrun --xscope test.xe
// for offline, detailed timing analysis (no chip needed):
//  xsim test.xe  --vcd-tracing "-o trace.vcd -tile tile[0] -ports -ports-detailed -clock-blocks -instructions -cores"


#include <stdio.h>
#include <stdint.h>
#include <xs1.h>
#include <syscall.h>
#include <timer.h>
#include <print.h>
#include <platform.h>
#include <xclib.h>

// XCORE -> RPI HAT:
// X0D10 -> Pin 11
// X0D11 -> Pin 9

// Define stepper motor connections and steps per revolution:

// Direction Pin is CK+ on TB6560
out port direction_port = on tile[0]: XS1_PORT_1C; // X0D10

// Step Pin is CLK+ on TB6560
out port step_port = on tile[0]: XS1_PORT_1D; // X0D11

#define stepsPerRevolution 400

int main (void) {

// Set spinning direction clockwise
direction_port <: 1;

// spin the stepper motor 1 revolution
  for (int i = 0; i < stepsPerRevolution; i++) {
  // These four lines result in 1 step:
  step_port <: 1;
  delay_microseconds(2000);
  step_port <: 0;
  delay_microseconds(2000);
  }

  return 0;
}
