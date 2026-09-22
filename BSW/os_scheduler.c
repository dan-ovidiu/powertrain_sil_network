#include "os_scheduler.h"
#include <stdio.h>
#include "../Engine_vECU/engine.h"
#include "../Transmission_vECU/transmission.h"
#include "../FMU_Plant/fmu_powertrain.h"
#include "../Virtual_CAN/vcan.h"
void OS_Start(void)
{
    VCAN_Init();
    Engine_Init();
    Transmission_Init();
    FMU_Init();
    
    printf("Step,Throttle,Gear,RPM,Speed\n");

    Virtual_CAN_Bus_t* bus = VCAN_GetBus();
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
        if(step % 1 == 0){
            Engine_Task();
        }

        if(step % 5 == 0){
            Transmission_Task();
        }

        FMU_DoStep();
        
        
        // --- READ MEASUREMENTS ---
        printf("%d,%.1f,%d,%.2f,%.2f\n", 
               step, 
               bus->throttle_pedal, 
               bus->current_gear, 
               bus->engine_rpm,
               bus->vehicle_speed_kmh);
    }
}