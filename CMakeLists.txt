cmake_minimum_required(VERSION 3.10)
project(Powertrain_SIL_Network)

set(SOURCES
    main.c
    Virtual_CAN/vcan.c
    Engine_vECU/engine.c
    Transmission_vECU/transmission.c
    FMU_Plant/fmu_powertrain.c
)

add_executable(network_sil ${SOURCES})
