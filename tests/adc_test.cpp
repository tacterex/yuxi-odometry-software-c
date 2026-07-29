#include <stdio.h>
#include "pico/stdlib.h"

#include "headers/peripherals.h"

int main()
{
    stdio_init_all();

    peripherals::EncoderReader axis_x(false);
    while (true) {
        printf("%d %d\n", axis_x.get_channel(), axis_x.get_raw_step());
        sleep_ms(200);
    }
}
