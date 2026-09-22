#ifndef COM_H
#define COM_H
float Com_ReceiveThrottle(void);
void Com_SendEnginePowerCmd(float power_cmd);
float Com_ReceiveEngineRpm(void);
void Com_SendCurrentGear(int gear);
#endif // COM_H