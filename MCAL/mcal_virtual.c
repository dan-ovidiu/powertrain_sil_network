#include "mcal_virtual.h"
#include "../FMU_Plant/fmu_powertrain.h"

void Mcal_PWM_SetDutyCycle(float duty_cycle){
    FMU_SetEnginePower(duty_cycle);
}