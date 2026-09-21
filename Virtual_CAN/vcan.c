#include "vcan.h"

// The physical memory location representing our SIL network
static Virtual_CAN_Bus_t global_can_bus;

void VCAN_Init(void) {
    global_can_bus.throttle_pedal = 0.0f;
    global_can_bus.engine_power_cmd = 0.0f;
    global_can_bus.engine_rpm = 0.0f;
    global_can_bus.current_gear = 1;
    global_can_bus.vehicle_speed_kmh = 0.0f;
}

Virtual_CAN_Bus_t* VCAN_GetBus(void) {
    return &global_can_bus;
}
