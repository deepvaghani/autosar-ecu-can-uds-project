#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "../include/swc_driver_alert.h"

// 🔹 SEND WARNING MESSAGE (0x123)
void sendWarning() {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    // Prepare SWC data
    SensorData input = {80.0, 9};
    WarningData output;

    Runnable_CheckDriverState(input, &output);

    // Prepare CAN frame
    frame.can_id = 0x123;
    frame.can_dlc = 8;
    memset(frame.data, 0, 8);
    frame.data[0] = output.warning_flag;
    frame.data[1] = output.fault_code;

    write(s, &frame, sizeof(frame));

    printf("[ECU1] Sent Warning (Fault: %d)\n", output.fault_code);

    close(s);
}

// 🔹 RECEIVE UDS RESPONSE (0x456)
void receiveResponse() {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    printf("[ECU1] Listening for UDS response...\n");

    int count = 0;

    while (count < 50) {  // loop ~5 seconds
        int nbytes = read(s, &frame, sizeof(frame));

        if (nbytes > 0) {
            printf("[ECU1 DEBUG] ID: 0x%X\n", frame.can_id);

            if (frame.can_id == 0x456) {
                printf("[ECU1] Received UDS Response: Fault = %d\n", frame.data[1]);
                close(s);
                return;
            }
        }

        usleep(100000); // 100 ms
        count++;
    }

    printf("[ECU1] ❌ No UDS response received (timeout)\n");

    close(s);
}

int main() {
    // Start listening FIRST
    if (fork() == 0) {
        receiveResponse();
        return 0;
    }

    sleep(1);  // give receiver time

    sendWarning();

    sleep(3);  // wait for response

    return 0;
}