#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

// 🔹 SEND UDS RESPONSE (0x456)
void sendUDSResponse(int fault_code) {
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

    frame.can_id = 0x456;
    frame.can_dlc = 8;
    memset(frame.data, 0, 8);

    // UDS Positive Response (0x62)
    frame.data[0] = 0x62;
    frame.data[1] = fault_code;

    write(s, &frame, sizeof(frame));

    printf("[ECU2] Sent UDS Response (Fault: %d)\n", fault_code);

    close(s);
}

// 🔹 RECEIVE WARNING FROM ECU1
void receiveCAN() {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // ❗ Disable own message reception
    int recv_own_msgs = 0;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS,
               &recv_own_msgs, sizeof(recv_own_msgs));

    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    printf("[ECU2] Waiting for message...\n");

    while (1) {
        read(s, &frame, sizeof(frame));

        printf("[ECU2 DEBUG] Received CAN ID: 0x%X\n", frame.can_id);

        if (frame.can_id == 0x123) {
            int warning = frame.data[0];
            int fault = frame.data[1];

            if (warning) {
                printf("[ECU2] Warning detected! Fault: %d\n", fault);
                sendUDSResponse(fault);
                break;
            }
        }
    }

    close(s);
}

int main() {
    receiveCAN();
    return 0;
}