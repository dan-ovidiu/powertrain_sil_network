#include "transmission.h"
#include "../Virtual_CAN/vcan.h"

static int internal_gear = 1;
static int shift_cooldown = 0; // Prevent shifting too fast

void Transmission_Init(void) {
    internal_gear = 1;
    shift_cooldown = 0;
}

void Transmission_Task(void) {
    // 1. Read Network Inputs (from Virtual CAN)
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    float rpm = bus->engine_rpm;
    
    // Manage cooldown timer
    if (shift_cooldown > 0) {
        shift_cooldown--;
    }
    
    // 2. Pure Math (Shift Logic)
    if (shift_cooldown == 0) {
        // Shift UP if RPM is too high
        if (rpm > 3500.0f && internal_gear < 3) {
            internal_gear++;
            shift_cooldown = 20; // Wait 20 cycles before shifting again
        }
        // Shift DOWN if RPM is too low
        else if (rpm < 1500.0f && internal_gear > 1) {
            internal_gear--;
            shift_cooldown = 20; 
        }
    }
    
    // 3. Write Network Output (Broadcast Gear to Virtual CAN)
    bus->current_gear = internal_gear;
}
