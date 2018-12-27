#define RH_PLATFORM_MBED RH_PLATFORM_MBED
#include <mbed.h>
#include <vector>
#include <RH_RF69.h>

//#define RH_TRANSMITTER

int main() {
    DigitalOut led(D6);
    led = 0;
    RH_RF69 rfm(D10, D9);
    bool success = false;
    size_t cnt = 0;
    Serial pc(D1, D0);
    pc.baud(9600);
    wait(1);
    uint8_t val = 0;
    while (!success) {
        success = rfm.init(val);
        pc.printf("RFM Init: %s Try %d\n\r", success ? "Succeeded" : "Failed", cnt++);
        pc.printf("Init Return Val: %x \n\r", val );
    }

#ifdef RH_TRANSMITTER
    cnt = 0;
    while(cnt<600) {
        uint8_t buf[4] = {0xAA, 0x55, 0xAA, 0x55};
        success = rfm.send(buf, sizeof(buf));
        pc.printf("RFM Send: %d\n\r", success);
        led = led ^ 1;
        cnt++;
        wait(1);
    }
    rfm.sleep();
    deepsleep();
#else
    uint8_t buf[32];
    uint8_t len = sizeof(buf);
    int avg = 0;
    cnt = 0;
    std::vector<int> last5(5);
    while(1) {
        pc.printf("recv... ");
        len = sizeof(buf);
        bool rc = rfm.recv(buf, &len);
        if (rc) {
            int8_t rssi = rfm.lastRssi();
            last5.erase(last5.begin()+4);
            last5.at(4) == 0 ? cnt = 5 : cnt =0;
            for( size_t p = 0; p < cnt; p++){
                last5.push_back(rssi);
            }
            avg = (last5.at(0)+last5.at(1)+last5.at(2)+last5.at(3)+last5.at(4))/5;            
            pc.printf("rssi=%d len=%u 0x", avg, len);
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