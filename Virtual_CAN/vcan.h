#ifndef VCAN_H
#define VCAN_H

// =========================================================================
// VIRTUAL CAN BUS (SIL Network Simulation)
// In a physical car, this would be a complex BSW module handling physical 
// voltages on copper wires (CAN_H, CAN_L). 
// In SIL, we simulate the network using a shared memory structure.
// =========================================================================

// Definition of the CAN messages exchanged on the network
typedef struct {
    float throttle_pedal;      // Sent by Driver (Python Test), Read by Engine vECU
    float engine_power_cmd;    // Sent by Engine vECU, Read by FMU Plant
    float engine_rpm;          // Sent by FMU Plant, Read by Transmission vECU
    int current_gear;          // Sent by Transmission vECU, Read by FMU Plant
    float vehicle_speed_kmh;   // Sent by FMU Plant, Read by Driver/Dashboard
} Virtual_CAN_Bus_t;

// Get a pointer to the simulated network
Virtual_CAN_Bus_t* VCAN_GetBus(void);

// Initialize the virtual network with zeros
void VCAN_Init(void);

#endif // VCAN_H
