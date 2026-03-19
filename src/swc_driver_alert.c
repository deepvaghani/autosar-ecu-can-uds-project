#include "swc_driver_alert.h"

void Runnable_CheckDriverState(SensorData input, WarningData* output) {
    if (input.drowsiness_level > 7 && input.speed > 60) {
        output->warning_flag = 1;
        output->fault_code = 0x01;
    } else {
        output->warning_flag = 0;
        output->fault_code = 0x00;
    }
}