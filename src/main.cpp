#define RH_PLATFORM_MBED RH_PLATFORM_MBED
#include <mbed.h>
#include <vector>
#include "RH_RF69.h"
#include "InterruptIn.h"
#include <PowerM.h>

//sudo miniterm /dev/ttyUSB0 115200
//parker@Parkers:~/.platformio/packages/framework-mbed/targets/TARGET_NXP/TARGET_LPC82X$

#define vect_Length 7
#define RH_TRANSMITTER
void wakeup(){
    Serial pc(D1, D0);
    pc.baud(115200);
    pc.printf("interrupt!!\r\n");
}

int main() {
    InterruptIn BTN(D3);
    DigitalOut led(D6);
    led = 0;    
    RH_RF69 rfm(D10, D9);
    bool success = false;
    size_t cnt = 0;
    Serial pc(D1, D0);
    pc.baud(115200);

    uint8_t val = 0;
    while (!success) {
        success = rfm.init(val);
        pc.printf("RFM Init: %s Try %d\r", success ? "Succeeded" : "Failed", cnt++);
    }
    pc.printf("\n\r");

            /*     Transmitter code     */
#ifdef RH_TRANSMITTER
    PowerM* PM = new PowerM();
    
    BTN.rise(wakeup);
    cnt = 0;
    while(1) {
        cnt++;
        uint8_t buf[4] = {0xAA, 0x55, 0xAA, 0x55};
        success = rfm.send(buf, sizeof(buf));
        pc.printf("RFM Send: %d\n\r", success);
        led = led ^ 1;
        rfm.sleep();
        wait(.25);
        if(cnt>11){
            cnt = 0;
            while(!rfm.sleep()){
                cnt++;
                pc.printf("RFM Sleep Failed. Try: %d\r", cnt);
            }
            pc.printf("RFM Sleep Successful!\r\n");
            wait(2);
            pc.printf("LPC Sleep init!\r\n");
            //PM->sleep();
            sleep();
            pc.printf("Awake!\r\n");
            cnt = 0;
        }
    }
    


            /*      Reciever Code      */
#else
    uint8_t buf[32];
    uint8_t len = sizeof(buf);
    int avg = 0;
    std::vector<int> last5(vect_Length);
    while(1) {
        pc.printf("\r                                  \r");
        pc.printf("recv... ");
        len = sizeof(buf);
        bool rc = rfm.recv(buf, &len);
        if (rc) {
            cnt = 0;
            int8_t rssi = rfm.lastRssi();
            last5.erase(last5.begin()+vect_Length-1);
            last5.push_back(rssi);
            avg = 0;
            for(size_t v = 0; v < last5.size(); v++){
                avg += last5.at(v);
            }
            avg = avg*10/vect_Length;
            pc.printf("rssi=%d len=%u 0x", avg, len);
            for (uint8_t i = 0; i < len; i++) {
                printf("%02x", buf[i]);
            }
            
        }
        else {
            pc.printf("failed! Try:%d\r", cnt++);
        }
        wait(0.5);
    }
#endif
}