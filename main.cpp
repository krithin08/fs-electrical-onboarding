 #include "etc_controller.h"
#include "mbed.h"
#include <cstdio>
#include <cstring>

ETCController etc(P10, P11);
CAN can(PB_8, PB_9, 500000);
Ticker etc_ticker;

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();

int main() {
    printf("Hello World!!\n");

    can.frequency(500000); // Set CAN frequency to 500kHz
    etc_ticker.attach(&send_etc_CAN_messages, 0.05f); // 20 Hz

    while (true) {
        etc.update_state();
    }

    return 0;
}

void send_etc_CAN_messages() {
    uint8_t data[8] = {0};

    // Ready-to-drive and motor-enabled bits are set high.
    data[0] = 0x03;
    data[1] = 0x00;

    // Example status payload for the ETC controller.
    // This keeps the CAN frame valid and can be extended with the
    // application-range and deviation bits as required by the project.
    CANMessage msg(0x180, data, 8);
    can.write(msg);
}

void send_sme_CAN_messages() {
    uint8_t data[8] = {0};
    data[0] = static_cast<uint8_t>(etc.motor_torque_demand & 0xFFU);

    CANMessage msg(0x200, data, 8);
    can.write(msg);
}

