#ifndef TB6560_h
#define TB6560_h

#include <stdint.h>
#include <stdlib.h>
#include <xs1.h>

class TB6560 {
	public:
	//constructors
	TB6560(out port enable_port, out port direction_port, out port step_port);
	void start();
	void stop();
	/// @brief Sets the speed (in RPM) of the stepper motor
	/// @param speed 
	void setSpeed(long speed);
	void setStepsPerRevolution(uint8_t stepsPerRevolution);
	void setDirection(int motordirection);
	void step(int number_of_steps);
	
	private :
	out port enable_port;
	out port direction_port;
	out port step_port; 

	long speed;
	uint8_t steps;
	unsigned long clock_delay;
	int direction;
	 unsigned long last_step_time;
};


#endif