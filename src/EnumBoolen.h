/*
  EnumBoolen.h - Control Enum Logic cross all RACE_UGV main function- Version 2
  Copyright (c) 2022 RFA.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
*/
#ifndef ENUMBOOLEN_H
#define ENUMBOOLEN_H
#include <Arduino.h>

enum Flow_Stage {
  Mode_0,
  Mode_1,
  Mode_2, // When OBS Outside Parameter
  Mode_3
  
};
//  extern enum Obs_State obsState;
inline Flow_Stage& flow_Stage() {
  static Flow_Stage currentflow_Stage;
  return currentflow_Stage;
}

enum Charge_Stage {
  Default_Mode,
  Idle,
  Constant_Current, // When OBS Outside Parameter
  Constant_Voltage
  
};
//  extern enum Obs_State obsState;
inline Charge_Stage& charge_Stage() {
  static Charge_Stage currentcharge_Stage;
  return currentcharge_Stage;
}

enum Battery_Stage {
  Min_Val,
  Inbetween,
  Max_Val, // When OBS Outside Parameter
  Finish
  
};
//  extern enum Obs_State obsState;
inline Battery_Stage& battery_Stage() {
  static Battery_Stage currentbattery_Stage;
  return currentbattery_Stage;
}

enum Battery_Status {
  Engage,
  Disengage

  
};
//  extern enum Obs_State obsState;
inline Battery_Status& battery_Status() {
  static Battery_Status currentbattery_Status;
  return currentbattery_Status;
}
#endif