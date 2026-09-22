#include "fmu_powertrain.h"
#include "../Virtual_CAN/vcan.h"

// Internal Physics State
static float vehicle_speed = 0.0f; // m/s
static float internal_rpm = 800.0f;
static float electrical_power_cmd = 0.0f;

void FMU_SetEnginePower(float power_cmd) {
    electrical_power_cmd = power_cmd;
}

void FMU_Init(void) {
    vehicle_speed = 0.0f;
    internal_rpm = 800.0f;
}

void FMU_DoStep(void) {
    // 1. Read Inputs
    float engine_power_percent = electrical_power_cmd; // 0.0 to 100.0%
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    int gear = bus->current_gear;
    
    // --- REALISTIC VEHICLE PHYSICS ---
    // Constants for a standard sedan (1500 kg)
    const float dt = 0.1f;              // Simulation step (100ms per step = 10s total simulation)
    const float vehicle_mass = 1500.0f; // kg
    const float max_engine_torque = 300.0f; // Nm (Newton-meters)
    const float final_drive_ratio = 3.5f;   // Differential ratio
    const float tire_radius = 0.3f;         // meters
    
    // 1. Calculate Engine Torque based on throttle pedal
    float engine_torque = (engine_power_percent / 100.0f) * max_engine_torque;
    
    // 2. Determine Transmission Gear Ratio
    float gear_ratio = 3.5f; // 1st Gear
    if (gear == 2) gear_ratio = 2.0f;
    if (gear == 3) gear_ratio = 1.2f;
    
    // 3. Calculate Force at the Wheels: F = (Torque * GearRatio * FinalDrive) / TireRadius
    float wheel_force = (engine_torque * gear_ratio * final_drive_ratio) / tire_radius;
    
    // 4. Calculate Aerodynamic Drag: Fd = 0.5 * rho * Cd * A * V^2 (Simplified to 0.4 * V^2)
    float drag_force = 0.4f * vehicle_speed * vehicle_speed; 
    
    // 5. Newton's Second Law: F = m * a  =>  a = F / m
    float acceleration = (wheel_force - drag_force) / vehicle_mass;
    
    // Update Vehicle Speed (v = v + a * dt)
    vehicle_speed += acceleration * dt;
    if (vehicle_speed < 0.0f) vehicle_speed = 0.0f;
    
    // 6. Calculate Engine RPM based on Vehicle Speed (locked by transmission)
    // RPM = (Speed * GearRatio * FinalDrive * 60) / (2 * pi * TireRadius)
    // Simplified constant: (60 / (2 * 3.1415 * 0.3)) = 31.83
    float rpm_multiplier = 31.83f * final_drive_ratio * gear_ratio;
    internal_rpm = 800.0f + (vehicle_speed * rpm_multiplier);
    
    // 7. Write Outputs (back to Virtual Environment)
    bus->engine_rpm = internal_rpm;
    bus->vehicle_speed_kmh = vehicle_speed * 3.6f;
}
