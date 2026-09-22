#include <stdio.h>
#include <stdlib.h>
#include "BSW/os_scheduler.h"

int main(int argc, char* argv[]) {
    float throttle_cmd = 100.0f; // Default throttle command (100%)

    if(argc > 1) {
        throttle_cmd = atof(argv[1]); // Convert first argument to float
    }

    OS_Start(throttle_cmd); // Start the OS with the specified throttle command
    return 0;
}
