#ifndef PROCESSES_H_
#define PROCESSES_H_

#include "headers/peripherals.h"

namespace processes {
    class PositionCalculator {
    private:
        peripherals::EncoderReader& _axis_x;
        peripherals::EncoderReader& _axis_y;
        peripherals::IMUReader& _imu;
        double pos_x, pos_y, phi;
        double dl_to_mm, dw_to_rps;
        double* pos_buffer;
        uint64_t previous_time, cycle_time;

        uint64_t current_time;
        double dx_absolute, dx_relative, dy_absolute, dy_relative, w, dt, dphi, s, c;

    public:
        PositionCalculator(
            peripherals::EncoderReader& u_axis_x,
            peripherals::EncoderReader& u_axis_y,
            peripherals::IMUReader& u_imu,
            double* u_pos_buffer
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
}

#endif