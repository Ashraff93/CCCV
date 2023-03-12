#include <stdint.h>
#include <Arduino.h>
#include "EnumBoolen.h" //#include "Mm.h" For Enum Logic
#include "Charger.h"


// other pins


// control loop tuning parameters
static const float Constant_V_Kp = 1; // tuned
static const float Constant_C_Kp = 1; // tuned
int PWMpin = 1; //Pin
int default_panelVpin = 1;
int default_panelIpin = 1;
int default_battVpin = 1;
int battEnable = 12;
int  duty = 255;

// target voltage for lead acid charge constant voltage output
float Max_Vtarget = 4.2; // 2.35V per cell
float targetFC = 4.0; // float charge 2.25V per cell

// for lead acid charge, the point when the battery is considered "discharged"
float ChargeCutoff = 3.3; // approx 50% SOC cutoff, 2.05V

// target/limit current on input side for lead acid
float default_Itarget = 0.8; // a bit above C/20 for 8Ah battery

//static const uint16_t Current_feedback = (uint16_t)(0.4 / default_currentADCscale); // a bit above C/20 for 8Ah battery

Charger charger(duty, default_panelVpin, default_panelIpin, default_battVpin);

void disengage( uint8_t dutyPtr) {
	digitalWrite(battEnable, LOW); // disconnect battery
	analogWrite(PWMpin, dutyPtr); // set so that PWMpin is always on, so MOSFET turns off
}

void engage( uint8_t dutyPtr)
{
	analogWrite(PWMpin, dutyPtr);
	delay(500);
	digitalWrite(battEnable, HIGH);
}
void setup() {
    pinMode(PWMpin, OUTPUT);
    Serial.begin(9600);
}
void loop() {

  charger.resetMPPT();
  charger.updateState();

  if(charger.battV <= ChargeCutoff){
    battery_Status()=Battery_Status::Disengage;
    disengage(255);
    // No Battery or bad battery,Ideal state
  }
   else if(charger.battV<targetFC && charger.battV>ChargeCutoff){


   battery_Status()=Battery_Status::Engage;
   engage(255);
   charge_Stage()=Charge_Stage::Constant_Current;

  }
  else if(charger.battV >=targetFC&& charger.battV<=Max_Vtarget){

  charge_Stage()=Charge_Stage::Constant_Voltage;

  }//charge battery in CV MODE

   else if(flow_Stage()==Flow_Stage::Mode_1){
    charge_Stage()=Charge_Stage::Constant_Voltage;
    flow_Stage()=Flow_Stage::Mode_0;
 

  }//
  else if(flow_Stage()==Flow_Stage::Mode_2){
    while(charger.battV >=targetFC){
      charger.updateState();
        if(charger.battV<targetFC){
          flow_Stage()=Flow_Stage::Mode_0;
          break;
        }
    }
  }

  if(flow_Stage()==Flow_Stage::Mode_1){
    charge_Stage()=Charge_Stage::Constant_Voltage;
    flow_Stage()=Flow_Stage::Mode_0;
  }// Battery enter in CV mde after CC mode
  else if(flow_Stage()==Flow_Stage::Mode_0){
    while(charger.battV >=4.0){
      charger.updateState();
        if(charger.battV<4){
          flow_Stage()=Flow_Stage::Mode_0;
    break;}}}// battery fully charged scanning for battery removed or not


//charge battery in CC MODE


 switch (charge_Stage()) {
      case 2:
            charger.updateDuty(charger.stepCC(Constant_C_Kp, targetFC), true);
            analogWrite(PWMpin, duty); 
            while(charger.battV<targetFC && charger.battV>=ChargeCutoff){
                charger.updateState();
                if(charger.battV>=targetFC){
                  flow_Stage()=Flow_Stage::Mode_1;
                  break;
                }
                else if(charger.battV<ChargeCutoff){
                  battery_Status()=Battery_Status::Disengage;
                  disengage(255);
                  break;
                }
            }
      break;
            case 3:
            charger.updateDuty(charger.stepCV(Constant_V_Kp, targetFC), true);
            analogWrite(PWMpin, duty); 
            while(charger.battV>=targetFC){
                charger.updateState();
                if (charger.getCurrent() < default_Itarget) {
                    flow_Stage()=Flow_Stage::Mode_2;
                    break;
                }    
                else if(charger.battV<ChargeCutoff){
                  battery_Status()=Battery_Status::Disengage;
                  disengage(255);
                  break;
                }
            }
      break;
     }


 }    