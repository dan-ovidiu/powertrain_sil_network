#include "fmu_powertrain.h"
#include "../Virtual_CAN/vcan.h"

// Internal Physics State
static float vehicle_speed = 0.0f; // m/s
static float internal_rpm = 800.0f;

void FMU_Init(void) {
    vehicle_speed = 0.0f;
    internal_rpm = 800.0f;
}

void FMU_DoStep(void) {
    // 1. Read Inputs (from Virtual Environment)
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    float engine_power = bus->engine_power_cmd; 
    int gear = bus->current_gear;
    
    // 2. Physics Equations
    float drag = vehicle_speed * 0.5f; 
    float acceleration = (engine_power - drag) * 0.05f; 
    
    vehicle_speed += acceleration;
    if (vehicle_speed < 0.0f) vehicle_speed = 0.0f;
    
    float gear_ratio = 50.0f; 
    if (gear == 2) gear_ratio = 30.0f;
    if (gear == 3) gear_ratio = 20.0f;
    
    internal_rpm = 800.0f + (vehicle_speed * gear_ratio);
    
    // 3. Write Outputs (back to Virtual Environment)
    bus->engine_rpm = internal_rpm;
    bus->vehicle_speed_kmh = vehicle_speed * 3.6f;
}
