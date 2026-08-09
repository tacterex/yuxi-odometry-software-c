from machine import Pin, I2C
from time import sleep
import struct

# --------------------------------------------------
# Configuration
# --------------------------------------------------

I2C_ID = 0
SDA_PIN = 0
SCL_PIN = 1
I2C_FREQ = 100_000

YUXI_ADDR = 0x69

RESET = 0x33
START_READ = 0x45

# --------------------------------------------------
# I2C initialization
# --------------------------------------------------

i2c = I2C(
    I2C_ID,
    sda=Pin(SDA_PIN),
    scl=Pin(SCL_PIN),
    freq=I2C_FREQ
)

# --------------------------------------------------
# Check connection
# --------------------------------------------------

if YUXI_ADDR not in i2c.scan():
    raise RuntimeError("YUXI Odometry Sensor not found")

print("YUXI connected.")

# --------------------------------------------------
# Reset and calibrate
# --------------------------------------------------

print("Resetting YUXI...")
i2c.writeto(YUXI_ADDR, bytes([RESET]))

print("Keep robot stationary during calibration.")
sleep(2)

print("Starting odometry...")

# --------------------------------------------------
# Main loop
# --------------------------------------------------

while True:
    i2c.writeto(YUXI_ADDR, bytes([START_READ]))
    data = i2c.readfrom(YUXI_ADDR, 12)

    pos_x, pos_y, heading = struct.unpack("<fff", data)

    print(
        f"X: {pos_x:8.2f} mm | "
        f"Y: {pos_y:8.2f} mm | "
        f"Heading: {heading:8.4f} rad"
    )

    sleep(0.05)