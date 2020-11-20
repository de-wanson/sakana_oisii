#include "arm.h"
#include "define.h"
Arm::Arm(uint8_t rf_1_in, uint8_t rf_2_in, uint8_t rf_p_in, uint8_t sw_in, uint8_t vol_in) {
	pinMode(rf_1 = rf_1_in, OUTPUT);
	pinMode(rf_2 = rf_2_in, OUTPUT);
	pinMode(rf_p = rf_p_in, OUTPUT);
	pinMode(sw = sw_in, INPUT);
	pinMode(vol = vol_in, INPUT);
}

void Arm::move(int rf_val) {
	this->target = rf_val;
}
void Arm::execute() {
	if(abs(pwm-target)<=smooth){
		 this->pwm = target;
	}else{
		this->pwm += (pwm<target)?smooth:-smooth;
	}
	if (pwm > 0) {
		digitalWrite(rf_1, HIGH);
		digitalWrite(rf_2, LOW);
	} else if (pwm < 0) {
		digitalWrite(rf_1, LOW);
		digitalWrite(rf_2, HIGH);
	} else {
		digitalWrite(rf_1, LOW);
		digitalWrite(rf_2, LOW);
	}
	analogWrite(rf_p, abs(pwm));
}