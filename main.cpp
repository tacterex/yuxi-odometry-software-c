#include <cstdint>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/mutex.h"
#include "pico/multicore.h"
#include <atomic>

#include "headers/data.h"
#include "headers/peripherals.h"
#include "headers/processes.h"

u_float_t pos[3];

peripherals::EncoderReader x(false), y(true);
peripherals::IMUReader imu(0, 2, 3, 4, 5);
processes::PositionCalculator calc(x, y, imu, pos, buffer_calc);
processes::I2CSlave slave(0, 0, 1, _i2c_slave_registers::ADDRESS, buffer_slave, &reinit_requsted_simple);

mutex_t data_mutex;

void core0_task();
void core1_task();

int main()
{
    stdio_init_all();


    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

void core0_loop() {
    
}