#define RH_PLATFORM_MBED RH_PLATFORM_MBED
#include <mbed.h>
#include <RH_RF69.h>

#define RH_TRANSMITTER

int main() {
    RH_RF69 rfm(D10, D9);
    bool success = false;
    int cnt = 0;
    while (!success) {
        success = rfm.init();
        printf("RFM Init: %s Try %d\n\r", success ? "Succeeded" : "Failed", cnt++);
    }

#ifdef RH_TRANSMITTER
    while(1) {
        uint8_t buf[4] = {0xAA, 0x55, 0xAA, 0x55};
        success = rfm.send(buf, sizeof(buf));
        printf("RFM Send: %d\n\r", success);
        wait(.25);
    }
#else
    uint8_t buf[32];
    uint8_t len = sizeof(buf);
    while(1) {
        printf("recv... ");
        len = sizeof(buf);
        bool rc = rfm.recv(buf, &len);
        if (rc) {
            int8_t rssi = rfm.lastRssi();
            printf("rssi=%d len=%u 0x", rssi, len);
            for (uint8_t i = 0; i < len; i++) {
                printf("%02x", buf[i]);
            }
            printf("\n\r");
        }
        else {
            printf("failed!\n\r");
        }
        wait(0.5);
    }
#endif
}