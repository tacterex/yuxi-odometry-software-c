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

        bool x_enabled, y_enabled, g_enabled;

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
        void enable_x();
        void enable_y();
        void enable_g();
        void disable_x();
        void disable_y();
        void disable_g();
        uint64_t get_cycle_time_us_64();
        void update_position();
    };

    class I2CSlave {
    private:
        uint8_t _i2c_i, _sda, _scl;
        i2c_inst_t* _i2c;
        uint8_t address;
        uint32_t baudrate;

        static I2CSlave* _instance;

        uint8_t* tx_buffer;
        slave_reg_t current_register;
        bool waiting_for_register;
        uint8_t* w_registers, * nw_registers;
        uint8_t temp_byte;
        bool& updated;

        static void _i2c_handler(i2c_inst_t* _r_i2c, i2c_slave_event_t event);
        void _handle_i2c_event(i2c_inst_t* _r_i2c, i2c_slave_event_t event);
        void _handle_i2c_receive();
        void _handle_i2c_request();
        void _handle_i2c_finish();

    public:
        I2CSlave(
            uint8_t _u_i2c_i,
            uint8_t _u_sda,
            uint8_t _u_scl,
            uint8_t _u_address,
            uint32_t _u_baudrate,
            uint8_t* _u_buffer,
            uint8_t* _u_w_registers,
            uint8_t* _u_nw_registers,
            bool& _u_updated
        );
        void set_current_slave_defult();
        void build();
    };
}

#endif