#include <stdio.h>
#include <stdlib.h>
#include "Virtual_CAN/vcan.h"
#include "Engine_vECU/engine.h"
#include "Transmission_vECU/transmission.h"
#include "FMU_Plant/fmu_powertrain.h"

int main(void) {
    // 1. Initialize System
    VCAN_Init();
    Engine_Init();
    Transmission_Init();
    FMU_Init();
    
    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
    
    // Print CSV Header for Python Testing
    printf("Step,Throttle,Gear,RPM,Speed\n");
    
    // 2. The Global Virtual OS Loop (runs for 100 steps)
    for (int step = 0; step < 100; step++) {
        
        // --- STIMULI INJECTION (From Python test script) ---
        // For testing, if step > 10, the driver pushes the gas pedal to 100%
        if (step > 10 && step < 60) {
            bus->throttle_pedal = 100.0f;
        } else if (step >= 60) {
            // Driver releases the gas pedal
            bus->throttle_pedal = 0.0f; 
        }
        
        // --- RUN DISTRIBUTED SYSTEM ---
        Engine_Task();
        FMU_DoStep();
        Transmission_Task();
        
        // --- READ MEASUREMENTS ---
        printf("%d,%.1f,%d,%.2f,%.2f\n", 
               step, 
               bus->throttle_pedal, 
               bus->current_gear, 
               bus->engine_rpm,
               bus->vehicle_speed_kmh);
    }
    
    return 0;
}
