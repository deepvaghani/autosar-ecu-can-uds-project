#ifndef SWC_DRIVER_ALERT_H
#define SWC_DRIVER_ALERT_H

typedef struct {
    float speed;
    int drowsiness_level;
} SensorData;

typedef struct {
    int warning_flag;
    int fault_code;
} WarningData;

void Runnable_CheckDriverState(SensorData input, WarningData* output);

#endif // SWC_DRIVER_ALERT_H