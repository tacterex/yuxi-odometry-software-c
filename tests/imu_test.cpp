#include <stdio.h>
#include "pico/stdlib.h"
#include <cstdint>

#include "headers/peripherals.h"

int main()
{
    stdio_init_all();

    sleep_ms(3000);

    peripherals::IMUReader imu(0, 2, 3, 4, 5);
    printf("%d\n\n", imu.get_who_am_i());
    imu.enable_gyroscope();
    sleep_ms(1000);
    imu.calibrate_offset_z(500);

    while (true) {
        printf("%d %d\n", imu.get_raw_gyroscope_z(true), imu.get_offset_z());
        sleep_ms(200);
    }
}
