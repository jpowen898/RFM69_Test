#define RH_PLATFORM_MBED RH_PLATFORM_MBED
#include <mbed.h>
#include <RH_RF69.h>

//#define RH_TRANSMITTER

int main() {
    DigitalOut led(D6);
    led = 0;
    RH_RF69 rfm(D10, D9);
    //rfm.setFrequency(500000);
    bool success = false;
    int cnt = 0;
    Serial pc(D1, D0);
    pc.baud(9600);
    wait(1);
    uint8_t val = 0;
    while (!success) {
        success = rfm.init(val);
        rfm.sleep();
        pc.printf("RFM Init: %s Try %d\n\r", success ? "Succeeded" : "Failed", cnt++);
        pc.printf("Init Return Val: %x \n\r", val );
    }

#ifdef RH_TRANSMITTER
    while(1) {
        uint8_t buf[4] = {0xAA, 0x55, 0xAA, 0x55};
        success = rfm.send(buf, sizeof(buf));
        pc.printf("RFM Send: %d\n\r", success);
        led = led ^ 1;
        wait(1);
    }
#else
    uint8_t buf[32];
    uint8_t len = sizeof(buf);
    while(1) {
        pc.printf("recv... ");
        len = sizeof(buf);
        bool rc = rfm.recv(buf, &len);
        if (rc) {
            int8_t rssi = rfm.lastRssi();
            pc.printf("rssi=%d len=%u 0x", rssi, len);
            for (uint8_t i = 0; i < len; i++) {
                printf("%02x", buf[i]);
            }
            pc.printf("\n\r");
        }
        else {
            pc.printf("failed!\n\r");
        }
        wait(0.5);
    }
#endif
}