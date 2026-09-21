#include "engine.h"
#include "../Virtual_CAN/vcan.h"

static float internal_rpm = 800.0f; // Idle RPM

void Engine_Init(void) {
    internal_rpm = 800.0f;
}

void Engine_Task(void) {
    // 1. Read Inputs (from Virtual CAN)
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    float throttle = bus->throttle_pedal;  // 0.0 to 100.0 %
    
    // 2. Engine Control Logic (e.g., Throttle mapping, torque limiting)
    // In a real ECU, this would be a massive map. We just pass it through.
    float power_cmd = throttle; 
    
    // 3. Write Output (Broadcast Power Command to FMU)
    bus->engine_power_cmd = power_cmd;
}
