# Distributed vECU Powertrain Network (SIL Simulation)

## Overview
This repository contains a **Software-in-the-Loop (SIL)** simulation of a distributed automotive powertrain network. It was developed to practically demonstrate the core concepts of the **AUTOSAR** layered architecture, Virtual ECUs (vECUs), and physical plant modeling (FMU). 

By decoupling the application logic from the hardware dependencies, this project allows a full C-based engine and transmission control network to be executed, simulated, and automatically tested on a standard Windows/Linux PC via CI/CD pipelines.

## Core Concepts Demonstrated

* **AUTOSAR Architecture Separation:** 
  * **Application Layer:** Pure control logic (`Engine_vECU`, `Transmission_vECU`) completely decoupled from hardware.
  * **RTE (Run-Time Environment):** Acts as the VFB (Virtual Functional Bus), routing signals between the applications, the network, and the physical actuators.
  * **BSW (Basic Software):** Contains the OS Scheduler (triggering 10ms and 50ms tasks) and the COM stack for network communication.
  * **MCAL (Microcontroller Abstraction Layer):** Simulates direct electrical PWM signals to bypass the network for physical actuators.
* **Virtual CAN Bus:** A simulated shared-memory struct that acts as the hardware network between the Engine ECU and Transmission ECU.
* **Physics Plant (FMU):** A mathematical model utilizing Newton's Second Law (`F=ma`), gear ratios, engine torque curves, and aerodynamic drag to simulate how the physical car reacts to the ECU commands.
* **Automated CI/CD Testing:** A headless Python test-runner that injects multiple driving scenarios (Drag Race, City, Highway) into the executable and automatically asserts requirements (e.g., max speed, gear shifts) on GitHub Actions.

## Project Structure

```text
powertrain_sil_network/
├── Engine_vECU/         # Engine App Layer (Calculates power command)
├── Transmission_vECU/   # Transmission App Layer (Calculates gear shifts)
├── RTE/                 # Routes signals between App, BSW, and MCAL
├── BSW/                 # OS Scheduler (RTOS) and COM Stack
├── MCAL/                # Virtual Hardware Drivers (PWM to injectors)
├── Virtual_CAN/         # Simulated hardware network bus
├── FMU_Plant/           # Physics model (Torque, Drag, Speed, RPM)
├── SIL_Simulation/      # Python test-runner and scenario injection
└── main.c               # Entry point (boots the OS Scheduler)
```

## How to Build and Run

### 1. Compile the C Executable
This project uses CMake. To build it on Windows (using MinGW):
```bash
cmake .
mingw32-make
```

### 2. Run the Automated Python SIL Tests
The Python script will execute the C program across three different throttle scenarios and generate data visualizations.
```bash
cd SIL_Simulation
python test_runner.py
```
*(Check the generated `sil_test_report_100.png` to view the physics simulation graphs!)*

## Future Improvements & Roadmap

While this project successfully demonstrates a base SIL environment, the following industry-standard improvements are planned:

1. **User-Friendly Dashboard (GUI):**
   * Implement a Python-based real-time dashboard (using PyQt or Tkinter) to act as a virtual cockpit. This will allow a tester to interactively control the throttle pedal with a slider and watch the RPM and Speed gauges react in real-time, bridging the gap between automated SIL testing and interactive HiL (Hardware-in-the-Loop) user experiences.
2. **FMI / FMU Standardization:**
   * Currently, the physics plant is a custom C file. In the future, this should be replaced by a compiled `.fmu` (Functional Mock-up Unit) exported from industry tools like MATLAB/Simulink or IPG CarMaker, utilizing the standard FMI API (`fmi2DoStep`).
3. **ARXML Generation:**
   * The RTE and BSW layers are currently hand-coded. A true AUTOSAR workflow involves auto-generating these `.c` files from an `.arxml` configuration file using tools like Vector DaVinci or dSPACE SystemDesk.
4. **ASAM XIL API:**
   * Upgrade the Python test script to utilize the ASAM XIL standard API for test automation, which is the industry standard for interacting with testbenches.
