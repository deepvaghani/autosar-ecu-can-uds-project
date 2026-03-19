#include "uds.h"
#include "can.h"
#include <stdio.h>

void sendVINResponse() {
    unsigned char response[8] = {0x62, 0xF1, 0x90, 'D', 'E', 'E', 'P', '1'};
    sendCAN(response);
}

void handleDiagnosticRequest(unsigned char *data) {
    if (data[0] == 0x22) {
        printf("UDS Request Received\n");
        sendVINResponse();
    }
}