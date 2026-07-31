#include <stdio.h>
#include "pico/stdlib.h"
#include <cstdint>

#include "headers/processes.h"
#include "headers/data.h"

int main()
{
    stdio_init_all();

    sleep_ms(3000);

    uint8_t test_buffer[24];
    for(uint8_t i = 0; i < 24; ++i)
        test_buffer[i] = i;
    bool test_sc = false;

    processes::I2CSlave slave(0, 0, 1, _i2c_slave_registers::ADDRESS, test_buffer, &test_sc);

    slave.build();
}
