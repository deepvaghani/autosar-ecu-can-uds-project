#include <stdio.h>
#include "rte.h"
#include "swc_driver_alert.h"

SensorData readSensors() {
    SensorData data = {80.0, 8};
    return data;
}

void sendWarning(WarningData output) {
    printf("Warning: %d | Fault Code: %d\n", output.warning_flag, output.fault_code);
}

void RTE_Run() {
    SensorData input = readSensors();
    WarningData output;

    Runnable_CheckDriverState(input, &output);
    sendWarning(output);
}