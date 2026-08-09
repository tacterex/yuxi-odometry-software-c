from machine import Pin, I2C
from time import sleep

I2C_ID = 0
SDA_PIN = 0
SCL_PIN = 1
I2C_FREQ = 100_000

YUXI_ADDR = 0x69
RESET = 0x33

i2c = I2C(
    I2C_ID,
    sda=Pin(SDA_PIN),
    scl=Pin(SCL_PIN),
    freq=I2C_FREQ
)

print("Resetting YUXI...")

i2c.writeto(YUXI_ADDR, bytes([RESET]))

print("Reset command sent.")
print("Keep the robot stationary during IMU calibration.")

sleep(2)

print("YUXI should now be ready.")