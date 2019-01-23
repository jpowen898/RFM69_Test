#ifndef POWER_MANAGER
#define POWER_MANAGER
#include <mbed.h>

class PowerM 
{
private:
    int *PCON = (int *) 0x40020000;
    int *SCR = (int *) 0xE000ED10;
    int *DPSLEEPCFG = (int *) 0x40048230;
    int *STARTERP0  = (int *) 0x40048204;
    int *MAINCLKSEL = (int *) 0x40048070;
public:
    PowerM(){};
    ~PowerM();

/*6.7.4.2 Programming Sleep mode
The following steps must be performed to enter Sleep mode:
1. The PM bits in the PCON register must be set to the default value 0x0.
2. The SLEEPDEEP bit in the ARM Cortex-M0+ SCR register must be set to zero
(Table 58).
3. Use the ARM Cortex-M0+ Wait-For-Interrupt (WFI) instruction.*/
    void sleep(){
        *PCON = *PCON & 0b000111111111;
        *SCR = *SCR & 0b11011;
        __WFI();
        //return true;    
    };

/*6.7.5.2 Programming Deep-sleep mode
The following steps must be performed to enter Deep-sleep mode:
1. The PM bits in the PCON register must be set to 0x1 (Table 61).
2. Select the power configuration in Deep-sleep mode in the PDSLEEPCFG (Table 52)
register.
3. Select the power configuration after wake-up in the PDAWAKECFG (Table 53)
register.
4. If any of the available wake-up interrupts are needed for wake-up, enable the
interrupts in the interrupt wake-up registers (Table 50, Table 51) and in the NVIC.
5. Select the IRC as the main clock. See Table 32.
6. Write one to the SLEEPDEEP bit in the ARM Cortex-M0+ SCR register (Table 58).
7. Use the ARM WFI instruction.*/
    bool DeepSleep(int NumberWakeUpInterrupts){
        *PCON = (*PCON & 0b000111111111) | 0b0100000000000;
        *DPSLEEPCFG = *DPSLEEPCFG | 0b000100000000000;
        /*int bin = 0b0;
        if (NumberWakeUpInterrupts >= 8){
            return false;
        }
        else{
            for (int i = 0; i < NumberWakeUpInterrupts; i++){
                bin >> 0b1;
            }
        }
        *STARTERP0 = bin;*/
        *STARTERP0 = 0b11111111;
        *MAINCLKSEL = 0x0;
        *SCR = *SCR | 0b00100;
        __WFI();
        return true;
    };

/*6.7.6.2 Programming Power-down mode
The following steps must be performed to enter Power-down mode:
1. The PM bits in the PCON register must be set to 0x2 (Table 61).
2. Select the power configuration in Power-down mode in the PDSLEEPCFG (Table 52)
register.
3. Select the power configuration after wake-up in the PDAWAKECFG (Table 53)
register.
4. If any of the available wake-up interrupts are used for wake-up, enable the interrupts
in the interrupt wake-up registers (Table 50, Table 51) and in the NVIC.
5. Select the IRC as the main clock. See Table 32.
6. Write one to the SLEEPDEEP bit in the ARM Cortex-M0+ SCR register (Table 58).
7. Use the ARM WFI instruction.*/
    bool PowerDown(){

    };

/*6.7.7.2 Programming Deep power-down mode using the WAKEUP pin:
The following steps must be performed to enter Deep power-down mode when using the
WAKEUP pin for waking up:
1. Pull the WAKEUP pin externally HIGH.
2. Ensure that bit 3 in the PCON register (Table 61) is cleared.
3. Write 0x3 to the PM bits in the PCON register (see Table 61).
4. Store data to be retained in the general purpose registers (Section 6.6.2).
5. Write one to the SLEEPDEEP bit in the ARM Cortex-M0+ SCR register (Table 58).
6. Use the ARM WFI instruction.*/
    bool DeepPowerDown(){

    };


};

#endif