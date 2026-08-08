#ifndef DATA_H_
#define DATA_H_

#include <cstdint>

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
    constexpr uint8_t _i2c_n_nw_regs      = 1;
    constexpr uint8_t _i2c_n_w_regs       = 3;

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

enum _imu_registers : uint8_t{
    WHO_AM_I    = 0x0F,
    CTRL2_G     = 0x11,
    OUTZ_L_G    = 0x26,
    OUTZ_H_G    = 0x27,
 
    // read/write bits 
    BIT_R_MASK  = 0x80,
    BIT_W_MASK  = 0x7F,

    // datasheet registers values
    ODR_G_104HZ = 0b0100 << 4,
    FS_G_500DPS = 0b01   << 2
};

enum _i2c_slave_registers : uint8_t {
    DEF_ADDR        = 0x00,

    ADDRESS         = 0x69,
    RESET_NW        = 0x33,
    X_ENABLED       = 0x34,
    Y_ENABLED       = 0x35,
    G_ENABLED       = 0x36,

    START_READ      = 0x45,
    READ_1          = 0x46,
    READ_2          = 0x47,
    READ_3          = 0x48,
    READ_4          = 0x49,
    READ_5          = 0x4A,
    READ_6          = 0x4B,
    READ_7          = 0x4C,
    READ_8          = 0x4D,
    READ_9          = 0x4E,
    READ_10         = 0x4F,
    READ_11         = 0x50
};

_i2c_slave_registers& operator++(_i2c_slave_registers& reg);

typedef float u_float_t;
constexpr uint8_t U_FLOAT_SIZE = sizeof(u_float_t);

typedef _imu_registers imu_reg_t;
typedef _i2c_slave_registers slave_reg_t;

#endif