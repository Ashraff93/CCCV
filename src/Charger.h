#ifndef CHARGER_H
#define CHARGER_H

#include <Arduino.h>
#include <stdint.h>

class Charger {
private:
	// // system parameters that won't change
	uint8_t  dutyPtr;
	uint8_t panelVpin, panelIpin, battVpin, inputpin;
public:
	// MPPT specific variables
	uint32_t prevPower;
	bool prevAdjustment; // 0 -> previously down, 1 -> previously up
	// state variables that will change when these functions are called, initialized to 0
	// also to be used by Protection
	uint16_t panelV, panelI, battV;

	Charger(uint8_t dutyPtr, uint8_t panelVpin, uint8_t panelIpin, uint8_t battVpin);


	void updateState();
	
	// reset MPPT state variables to 0
	void resetMPPT();
	
	// updates the value stored at dutyPtr according to adjustment, returns 1 if success, 0 if hit duty deadzone
	// if invert is true, for a positive adjustment input, will apply a negative change to duty cycle
	// if duty deadzone was going to be reached (<=9 or >=245), will set to 10 or 244 instead
	// assumes 9 < duty < 245 initially
	// does NOT update the analogWrite
	bool updateDuty(int8_t adjustment, bool invert = true);
	
	// return currently stored voltage value, in ADC units
	uint16_t getInVoltage();
	
	// return currently stored voltage value, in ADC units
	uint16_t getOutVoltage();
	
	// return currently stored current value, in ADC units
	uint16_t getCurrent();
	
	// from current state variables, returns the requested adjustment to duty cycle
	// sign: if current voltage is under target, will return positive value
	int8_t stepCV(float Kp, uint16_t scaledVtarget);
	// maximum size of requested adjustment is 20
	// no time delay
	// a modified proportional (P) controller

	// from current state variables, returns the requested adjustment to duty cycle
	// sign: if current current is under target, will return positive value
	int8_t stepCC(float Kp, uint16_t scaledItarget);
	// maximum size of requested adjustment is 20
	// no time delay
	// a modified proportional (P) controller 

	// from current state variables, returns the requested adjustment to duty cycle
	// sign: same as CV and CC
	int8_t stepMPPT(uint16_t scaledSoftIlimit);
	// maximum size of requested adjustment is 1
	// a perturb-and-observe MPPT implementation
	// no time delay
	// updates prevPower and prevAdjustment for next iteration

};


#endif