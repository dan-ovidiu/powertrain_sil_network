#ifndef RTE_H
#define RTE_H

float Rte_Read_ThrottlePedal(void);
void Rte_Write_EnginePowerCmd(float power_cmd);

float Rte_Read_EngineRpm(void);
void Rte_Write_CurrentGear(int gear);

#endif // RTE_H