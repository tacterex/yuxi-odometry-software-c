from machine import Pin, I2C

I2C_ID = 0
SDA_PIN = 0
SCL_PIN = 1
I2C_FREQ = 100_000

YUXI_ADDR = 0x69

X_ENABLED = 0x34
Y_ENABLED = 0x35
G_ENABLED = 0x36

i2c = I2C(
    I2C_ID,
    sda=Pin(SDA_PIN),
    scl=Pin(SCL_PIN),
    freq=I2C_FREQ
)

# Comment specific line to disable only one sensor

# Disable X-axis
i2c.writeto(YUXI_ADDR, bytes([X_ENABLED, 0x01]))

# Disable Y-axis
i2c.writeto(YUXI_ADDR, bytes([Y_ENABLED, 0x01]))

# Disable gyro
i2c.writeto(YUXI_ADDR, bytes([G_ENABLED, 0x01]))

print("All sensor axes enabled.")