#ifndef DATA_H_
#define DATA_H_

#include <cstdint>

typedef float u_float_t;
constexpr uint8_t U_FLOAT_SIZE = sizeof(u_float_t);

namespace hardware{
    constexpr uint8_t  adc0_gpio          = 26;
    constexpr uint16_t encoder_max_value  = (0b1 << 12) - 1;
    constexpr uint16_t imu_max_value      = 0x10000 - 1;
    constexpr uint16_t imu_half_max_value = 0x08000;

    constexpr uint8_t LED_RED_GPIO        = 18;
    constexpr uint8_t LED_GREEN_GPIO      = 19;
    constexpr uint8_t LED_BLUE_GPIO       = 20;

    constexpr uint8_t _i2c_i              = 0;
    constexpr uint8_t _i2c_sda            = 0;
    constexpr uint8_t _i2c_scl            = 1;
    constexpr uint32_t _i2c_baudrate      = 100 * 1000;

    constexpr uint8_t _spi_i              = 0;
    constexpr uint8_t _spi_scl            = 2;
    constexpr uint8_t _spi_miso           = 3;
    constexpr uint8_t _spi_mosi           = 4;
    constexpr uint8_t _spi_cs_pin         = 5;
    constexpr uint32_t _spi_baudrate      = 2000 * 1000;

    constexpr uint16_t _g_cali_samples    = 1200;
}

namespace mechanical{
    constexpr uint8_t WHEEL_DIAMETER_MM = 50;
}

namespace imu_registers{
    constexpr uint8_t WHO_AM_I    = 0x0F;
    constexpr uint8_t CTRL2_G     = 0x11;
    constexpr uint8_t OUTZ_L_G    = 0x26;
    constexpr uint8_t OUTZ_H_G    = 0x27;
 
    // read/write bits 
    constexpr uint8_t BIT_R_MASK  = 0x80;
    constexpr uint8_t BIT_W_MASK  = 0x7F;

    // datasheet registers values
    constexpr uint8_t ODR_G_104HZ = 0b0100 << 4;
    constexpr uint8_t FS_G_500DPS = 0b01   << 2;
}

namespace _i2c_slave_registers {
    constexpr uint8_t ADDRESS         = 0x69;
    constexpr uint8_t START_READ      = 0x45;
    constexpr uint8_t RESET_NW        = 0x33;
    constexpr uint8_t X_ENABLED       = 0x34;
    constexpr uint8_t Y_ENABLED       = 0x35;
    constexpr uint8_t G_ENABLED       = 0x36;
}

#endif