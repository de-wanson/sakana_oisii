#ifndef _ARM_H_
#define _ARM_H_
#include <Arduino.h>
class Arm {
	public:
	uint8_t rf_1;
	uint8_t rf_2;
	uint8_t rf_p;
	uint8_t sw;
	int pwm = 0;
	int target = 0;
	uint8_t vol;
	Arm(uint8_t rf_1_in, uint8_t rf_2_in, uint8_t rf_p_in, uint8_t sw_in, uint8_t vol_in);
	void move(int rf_val);
	void execute();
};

#endif