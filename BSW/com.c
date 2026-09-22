#include "com.h"
#include "../Virtual_CAN/vcan.h"
float Com_ReceiveThrottle(void){
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    return bus->throttle_pedal;
}

void Com_SendEnginePowerCmd(float power_cmd){
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    bus->engine_power_cmd = power_cmd;
}

float Com_ReceiveEngineRpm(void){
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    return bus->engine_rpm;
}

void Com_SendCurrentGear(int gear){
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    bus->current_gear = gear;
}