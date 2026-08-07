#include <stdio.h>
#include "pico/stdlib.h"
#include <cstdint>

#include "headers/peripherals.h"
#include "headers/processes.h"
#include "headers/data.h"

int main()
{
    stdio_init_all();

    peripherals::leds_init();
    peripherals::leds_turn_on();

    sleep_ms(3000);

    uint8_t test_buffer[U_FLOAT_SIZE * 3];
    for(uint8_t i = 0; i < U_FLOAT_SIZE * 3; ++i)
        test_buffer[i] = i;
    bool test_w_regs[3], test_nw_regs[1];
    bool test_sc = false;

    processes::I2CSlave slave(0, 0, 1, _i2c_slave_registers::ADDRESS, hardware::_i2c_baudrate, test_w_regs, test_nw_regs, test_sc);

    slave.build();

    peripherals::leds_turn_blue();

    uint64_t c = 0;
    while(true) {
        printf("%d", test_sc);
    }
}
