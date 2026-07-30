#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include <cstdint>
#include "hardware/spi.h"

namespace peripherals {
    class EncoderReader {
    private:
        uint8_t  _adc_channel;
        uint16_t _last_position;
        uint16_t _max_value;
        void update_last_position();

    public:
        EncoderReader(bool XY);
        uint16_t get_raw_position();
        int16_t  get_raw_step();
        uint16_t get_max_value();
        uint8_t  get_channel();
    };

    class IMUReader {
    private:
        uint8_t _spi_i;
        spi_inst_t* _spi;
        uint8_t _cs_pin;
        bool gyro_enabled;
        int16_t offset_z;
        uint8_t odr;
        uint8_t dps;
        void select();
        void deselect();
    
    public:
        IMUReader(uint8_t _u_spi_i, uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t _u_cs_pin);
        uint8_t get_raw_refresh_rate();
        uint8_t get_raw_sensivity();
        uint16_t get_refresh_rate();
        uint16_t get_sensivity();
        int16_t get_offset_z();
        uint8_t _read_register(uint8_t reg);
        void _write_register(uint8_t reg, uint8_t value);
        void _read_registers(uint8_t reg, uint8_t* buffer, uint8_t n_bytes);

        uint8_t get_who_am_i();
        void enable_gyroscope();
        void calibrate_offset_z(uint16_t n_samples);
        int16_t get_raw_gyroscope_z(bool include_offset);
    };
}

#endif