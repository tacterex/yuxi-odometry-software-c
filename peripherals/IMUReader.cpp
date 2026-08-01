#include <cstdint>
#include "headers/peripherals.h"
#include "headers/data.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

using namespace peripherals;

spi_inst_t* get_spi(uint8_t i){
    return spi0;
}

IMUReader::IMUReader(
    uint8_t _u_spi_i,
    uint8_t sck,
    uint8_t miso,
    uint8_t mosi,
    uint8_t _u_cs_pin
):
    _spi_i(_u_spi_i),
    _cs_pin(_u_cs_pin)
{
    _spi = get_spi(_u_spi_i);

    spi_init(_spi, hardware::_spi_baudrate);
    spi_set_format(
        _spi,
        8,
        SPI_CPOL_1,
        SPI_CPHA_1,
        SPI_MSB_FIRST
    );

    gpio_set_function(sck, GPIO_FUNC_SPI);
    gpio_set_function(miso, GPIO_FUNC_SPI);
    gpio_set_function(mosi, GPIO_FUNC_SPI);

    gpio_init(_cs_pin);
    gpio_set_dir(_cs_pin, GPIO_OUT);
    gpio_put(_cs_pin, 1);

    odr = imu_registers::ODR_G_104HZ;
    dps = imu_registers::FS_G_500DPS;
    gyro_enabled = false;

    offset_z = 0;
}

void IMUReader::select() { 
    gpio_put(_cs_pin, 0); 
}

void IMUReader::deselect() {
    gpio_put(_cs_pin, 1);
}

uint8_t IMUReader::get_raw_refresh_rate() {
    return odr;
}

uint8_t IMUReader::get_raw_sensivity() {
    return dps;
}

uint16_t IMUReader::get_refresh_rate() {
    if ((~odr) ^ imu_registers::ODR_G_104HZ)
        return 104;
    return 0;
}

uint16_t IMUReader::get_sensivity() {
    if ((~dps) ^ imu_registers::FS_G_500DPS)
        return 500;
    return 500;
}

int16_t IMUReader::get_offset_z() {
    return offset_z;
}

uint8_t IMUReader::_read_register(uint8_t reg) {
    uint8_t tx[2] = {uint8_t(reg | imu_registers::BIT_R_MASK), 0};
    uint8_t rx[2];

    select();
    spi_write_read_blocking(_spi, tx, rx, 2);
    deselect();

    return rx[1];
}

void IMUReader::_write_register(uint8_t reg, uint8_t val) {
    uint8_t tx[2] = {uint8_t(reg & imu_registers::BIT_W_MASK), val};

    select();
    spi_write_blocking(_spi, tx, 2);
    deselect();
}

void IMUReader::_read_registers(uint8_t reg, uint8_t* buffer, uint8_t n_bytes) {
    uint8_t tx[n_bytes + 1];
    uint8_t rx[n_bytes + 1];

    tx[0] = reg | imu_registers::BIT_R_MASK;
    for(uint8_t i = 0; i < n_bytes; ++i){
        tx[i + 1] = 0x00;
    }

    select();
    spi_write_read_blocking(_spi, tx, rx, n_bytes + 1);
    deselect();

    for(uint8_t i = 0; i < n_bytes; ++i){
        buffer[i] = rx[i+1];
    }
}

uint8_t IMUReader::get_who_am_i() {
    return _read_register(imu_registers::WHO_AM_I);
}

void IMUReader::enable_gyroscope() {
    _write_register(imu_registers::CTRL2_G, odr | dps);
    gyro_enabled = true;
}

void IMUReader::calibrate_offset_z(uint16_t n_samples) {
    if(gyro_enabled){
        int32_t s = 0;
        for(uint16_t i = 0; i < n_samples; ++i){
            s += get_raw_gyroscope_z(false);
        }
        offset_z = s / n_samples;
    }
}

int16_t IMUReader::get_raw_gyroscope_z(bool include_offset) {
    uint8_t reg_readings[2];

    _read_registers(imu_registers::OUTZ_L_G, reg_readings, 2);

    uint16_t raw = (reg_readings[1] << 8) | reg_readings[0];
    int16_t res = static_cast<int16_t>(raw);

    if(include_offset) res -= offset_z;

    return res;
}