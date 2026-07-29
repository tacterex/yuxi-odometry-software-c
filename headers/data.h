#ifndef DATA_H_
#define DATA_H_

#include <cstdint>

namespace hardware{
    const uint8_t adc0_gpio = 26;
}

namespace mechanical{
    const uint8_t WHEEL_DIAMETER_MM = 50;
}

namespace imu_registers{
    const uint8_t WHO_AM_I    = 0x0F;
    const uint8_t CTRL2_G     = 0x11;
    const uint8_t OUTZ_L_G    = 0x26;
    const uint8_t OUTZ_H_G    = 0x27;
 
    // read/write bits 
    const uint8_t BIT_R_MASK  = 0x80;
    const uint8_t BIT_W_MASK  = 0x7F;

    // datasheet registers values
    const uint8_t ODR_G_104HZ = 0b0100 << 4;
    const uint8_t FS_G_500DPS = 0b01   <<2;
}

#endif