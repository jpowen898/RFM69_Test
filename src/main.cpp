#define RH_PLATFORM_MBED RH_PLATFORM_MBED
#include <mbed.h>
#include <RH_RF69.h>

int main() {
    RH_RF69 rfm(D10, D9);
    // put your setup code here, to run once:
        int count = 0;
        bool success = rfm.init();
        printf("RFM Init: %d\n\r", success);
    while(1) {
        uint8_t buf[4] = {0xAA, 0x55, 0xAA, 0x55};
        success = rfm.send(buf, 4);
        printf("RFM Send: %d\n\r", success);
        count++;
        wait(.25);
        // put your main code here, to run repeatedly:
    }
}