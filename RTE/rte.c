#include "rte.h"
#include "../BSW/com.h"
#include "../MCAL/mcal_virtual.h"

float Rte_Read_ThrottlePedal(void) {
    return Com_ReceiveThrottle();
}

void Rte_Write_EnginePowerCmd(float power_cmd) {
    Mcal_PWM_SetDutyCycle(power_cmd);
}

float Rte_Read_EngineRpm(void) {
    return Com_ReceiveEngineRpm();
}

void Rte_Write_CurrentGear(int gear) {
    Com_SendCurrentGear(gear);
}