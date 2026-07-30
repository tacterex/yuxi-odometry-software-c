#include <stdio.h>
#include "pico/stdlib.h"
#include <cstdint>

#include "headers/peripherals.h"

int main()
{
    stdio_init_all();

    peripherals::leds_init();

    while (true) {
        peripherals::leds_turn_blue();
        sleep_ms(800);
        peripherals::leds_turn_yellow();
        sleep_ms(800);
        peripherals::leds_turn_off();
        sleep_ms(800);
    }
}
