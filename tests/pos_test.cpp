#include <stdio.h>
#include "pico/stdlib.h"
#include <cstdint>

#include "headers/peripherals.h"
#include "headers/processes.h"
#include "headers/data.h"

int main()
{
    stdio_init_all();

    sleep_ms(3000);

    u_float_t pos[3] = {0, 0, 0};
    uint8_t pos_raw[3*U_FLOAT_SIZE] = {0};

    peripherals::EncoderReader x(false);
    peripherals::EncoderReader y(true);
    peripherals::IMUReader imu(0, 2, 3, 4, 5);

    processes::PositionCalculator calc(x, y, imu, pos, pos_raw);

    while (true) {
        calc.update_position();
        printf("%10.2f %10.2f %10.2f %d\n", pos[0], pos[1], pos[2], calc.get_cycle_time_us_64());
    }
}
