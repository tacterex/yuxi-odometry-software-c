from machine import Pin, I2C
from time import sleep
import struct

I2C_ID = 0
SDA_PIN = 0
SCL_PIN = 1
I2C_FREQ = 100_000

YUXI_ADDR = 0x69
START_READ = 0x45

i2c = I2C(
    I2C_ID,
    sda=Pin(SDA_PIN),
    scl=Pin(SCL_PIN),
    freq=I2C_FREQ
)

while True:
    # Select the first odometry register
    i2c.writeto(YUXI_ADDR, bytes([START_READ]))

    # Read 12 consecutive bytes
    data = i2c.readfrom(YUXI_ADDR, 12)

    # Decode three little-endian float32 values
    pos_x, pos_y, heading = struct.unpack("<fff", data)

    print(
        f"X: {pos_x:8.2f} mm | "
        f"Y: {pos_y:8.2f} mm | "
        f"Heading: {heading:8.4f} rad"
    )

    sleep(0.05)