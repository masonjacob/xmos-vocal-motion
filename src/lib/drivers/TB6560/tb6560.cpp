#include "tb6560.h"

TB6560(out port enable_port, out port direction_port, out port step_port)
{
	this->step_port=step_port;
	this->direction_port=direction_port;
  this->enable_port=enable_port;
	this->last_step_time = 0; // time stamp in us of the last step take
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//											Functions for setting up the speed

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TB6560::setStepsPerRevolution(uint8_t stepsPerRevolution)
{
	this->steps=stepsPerRevolution;
}


void TB6560::setDirection(int motordirection)
{
	this-> direction=motordirection;
	
	if(this-> direction==1)
	{
		direction_port <: 1;
	}
	else if(this-> direction==0)
	{
		direction_port <: 0;
	}
}

//		** This will set the speed in revolutions per minute **
	

void TB6560::setSpeed(long speed)
{
	this->speed=speed;
	this->clock_delay = (60L * 1000L * 1000L )/ this->steps / this->speed;
	this->clock_delay =this->clock_delay / 2;
	
}

void TB6560::start()
{
  step_port <: 1;
	delay_microseconds(this->clock_delay);
	step_port <: 0;
	delay_microseconds(this->clock_delay);
} 

void TB6560::stop()
{
	step_port <: 0;
	delay_microseconds(this->clock_delay);
}
 
void TB6560::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move);  // how many steps to take
   // determine direction based on whether steps_to_mode is + or -:
   /*
  if (steps_to_move > 0) { 
  this->direction = 1;
  digitalWrite(this->direction_pin,HIGH);
  
	}
  if (steps_to_move < 0) { 
  this->direction = 0; 
  digitalWrite(this->direction_pin,LOW);
  }
  */

  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    timer t;
    unsigned long now;
    t :> now;

    // move only if the appropriate delay has passed:
    if (now - this->last_step_time >= this->clock_delay)
    {
      // get the timeStamp of when you stepped:
      this->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1)
      {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
     start();
    }
  }
}


