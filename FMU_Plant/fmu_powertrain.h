#ifndef FMU_POWERTRAIN_H
#define FMU_POWERTRAIN_H

// Initialize the physics model
void FMU_Init(void);

// Step the physical simulation forward in time (dt = 0.1s)
void FMU_DoStep(void);

void FMU_SetEnginePower(float power_cmd);
#endif
