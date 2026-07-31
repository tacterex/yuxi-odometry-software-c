#ifndef PROCESSES_H_
#define PROCESSES_H_

#include "headers/peripherals.h"
#include "hardware/i2c.h"
#include "pico/i2c_slave.h"
#include "headers/data.h"

namespace processes {
    class PositionCalculator {
    private:
        peripherals::EncoderReader& _axis_x;
        peripherals::EncoderReader& _axis_y;
        peripherals::IMUReader& _imu;
        u_float_t pos_x, pos_y, phi;
        u_float_t dl_to_mm, dw_to_rps;
        u_float_t* pos_buffer;
        uint8_t* pos_raw_buffer;
        uint64_t previous_time, cycle_time;

        uint64_t current_time;
        u_float_t dx_absolute, dx_relative, dy_absolute, dy_relative, w, dt, dphi, s, c;

    public:
        PositionCalculator(
            peripherals::EncoderReader& u_axis_x,
            peripherals::EncoderReader& u_axis_y,
            peripherals::IMUReader& u_imu,
            u_float_t* u_pos_buffer,
            uint8_t* u_pos_raw_buffer
        );
        ~PositionCalculator();
        void init_encoder();
        void init_led();
        void init_imu();
        void init_timer();
        void reset();
        void reinit();
        void stop();
        uint64_t get_cycle_time_us_64();
        void update_position();
    };

    class I2CSlave {
    private:
        uint8_t _i2c_i;
        i2c_inst_t* _i2c;
        uint8_t _address;
        static void _i2c_handler(i2c_inst_t* _r_i2c, i2c_slave_event_t event);
        void _handle_i2c_event(i2c_inst_t* _r_i2c, i2c_slave_event_t event);
        static I2CSlave* _instance;
        uint8_t* _tx_buffer;
        uint8_t _current_register;
        bool* _special_request_pointer;
        uint8_t _sda, _scl;

    public:
        I2CSlave(
            uint8_t _u_i2c_i,
            uint8_t _u_sda,
            uint8_t _u_scl,
            uint8_t _u_address,
            uint8_t* _u_buffer,
            bool* _u_special_request_pointer
        );
        void set_current_slave_defult();
        void build();
    };
}

#endif