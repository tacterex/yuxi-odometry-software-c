#include <cstdint>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/mutex.h"
#include "pico/multicore.h"
#include <cstring>

#include "headers/data.h"
#include "headers/peripherals.h"
#include "headers/processes.h"

static void core1_task();

struct shared_data
{
    uint8_t buffer[3 * U_FLOAT_SIZE];
    bool reinit_requested;
} calc_data, slave_data, cmn_data;


u_float_t pos[3];

peripherals::EncoderReader x(false), y(true);
peripherals::IMUReader imu(
    hardware::_spi_i,
    hardware::_spi_scl,
    hardware::_spi_miso, 
    hardware::_spi_mosi,
    hardware::_spi_cs_pin
);
processes::PositionCalculator calc(
    x,
    y,
    imu,
    pos,
    calc_data.buffer
);
processes::I2CSlave slave(
    hardware::_i2c_i, 
    hardware::_i2c_sda, 
    hardware::_i2c_scl, 
    _i2c_slave_registers::ADDRESS, 
    slave_data.buffer, 
    slave_data.reinit_requested
);

mutex_t data_mutex;

int main()
{
    stdio_init_all();

    mutex_init(&data_mutex);

    calc.reinit();

    sleep_ms(500);

    multicore_launch_core1(core1_task);
    
    while(true) {
        calc.update_position();

        mutex_enter_blocking(&data_mutex);

        if (slave_data.reinit_requested){
            calc_data.reinit_requested = true;
            slave_data.reinit_requested = false;
        }

        std::memcpy(&slave_data.buffer, &calc_data.buffer, 3 * U_FLOAT_SIZE);

        mutex_exit(&data_mutex);

        if(calc_data.reinit_requested) {
            calc.reinit();
            calc_data.reinit_requested = false;
        }
    }
}

void core1_task() {
    peripherals::leds_turn_off();
    sleep_ms(500);
    slave.build();
    peripherals::leds_turn_yellow();
    while(true) tight_loop_contents();
}