#include "headers/peripherals.h"
#include "headers/processes.h"
#include "headers/data.h"
#include <cmath>
#include "pico/stdlib.h"
#include "pico/time.h"
#include <cstring>

using namespace peripherals;
using namespace processes;

constexpr u_float_t PI = 3.1415926f;

PositionCalculator::PositionCalculator(EncoderReader& u_axis_x,
     EncoderReader& u_axis_y, IMUReader& u_imu, u_float_t* u_pos_buffer, uint8_t* u_pos_raw_buffer):
     _axis_x(u_axis_x), _axis_y(u_axis_y), _imu(u_imu), pos_buffer(u_pos_buffer), pos_raw_buffer(u_pos_raw_buffer) {
        reinit();
}

void PositionCalculator::init_encoder() {
    _axis_x.reinit();
    _axis_y.reinit();
}

void PositionCalculator::init_led() {
    leds_init();
    leds_turn_on();
}

void PositionCalculator::init_imu(){
    _imu.get_who_am_i();
    _imu.enable_gyroscope();
    sleep_ms(1000);
    _imu.calibrate_offset_z(hardware::_g_cali_samples);
}

void PositionCalculator::init_timer() {
    previous_time = time_us_64();
}

void PositionCalculator::reset() {
    pos_x = pos_y = phi = previous_time = cycle_time = 0;
    dl_to_mm = PI * mechanical::WHEEL_DIAMETER_MM / hardware::encoder_max_value;
    dw_to_rps = 2.0f * _imu.get_sensivity() / hardware::imu_half_max_value * PI / 180;
}

void PositionCalculator::reinit() {
    leds_init();
    leds_turn_blue();

    init_encoder();
    init_imu();
    reset();
    init_timer();

    leds_turn_yellow();
}

void PositionCalculator::stop() {
    leds_turn_on();
}

uint64_t PositionCalculator::get_cycle_time_us_64() {
    return cycle_time;
}

void PositionCalculator::update_position() {
    dx_relative = _axis_x.get_raw_step()         * dl_to_mm;
    dy_relative = _axis_y.get_raw_step()         * dl_to_mm;
    w           = _imu.get_raw_gyroscope_z(true) * dw_to_rps;

    current_time = time_us_64();
    cycle_time = current_time - previous_time;
    dt = cycle_time * 1e-6;
    dphi = w * dt;
    previous_time = current_time;

    phi += dphi;
    s = sin(phi);
    c = cos(phi);

    dx_absolute = dx_relative * c - dy_relative * s;
    dy_absolute = dx_relative * s + dy_relative * c;

    pos_x += dx_absolute;
    pos_y += dy_absolute;

    pos_buffer[0] = pos_x;
    pos_buffer[1] = pos_y;
    pos_buffer[2] = phi;

    std::memcpy(pos_raw_buffer, pos_buffer, 3 * U_FLOAT_SIZE);
}

PositionCalculator::~PositionCalculator() {
    leds_turn_on();
}