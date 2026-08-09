from machine import Pin, I2C
from time import sleep

I2C_ID = 0
SDA_PIN = 0
SCL_PIN = 1
I2C_FREQ = 100_000

i2c = I2C(
    I2C_ID,
    sda=Pin(SDA_PIN),
    scl=Pin(SCL_PIN),
    freq=I2C_FREQ
)

print("Scanning I2C bus...")

devices = i2c.scan()

if 0x69 in devices:
    print("YUXI Odometry Sensor found at 0x69")
else:
    print("YUXI Odometry Sensor not found")

print("Detected devices:", [hex(addr) for addr in devices])