# YUXI ODOMETRY SOFTWARE DOCUMENTATION

## External References

* For **hardware documentation**, see the [YUXI Odometry Hardware repository](https://github.com/tacterex/yuxi-odometry-hardware).
* For the **FTC SDK library**, see: **TBA**

## Short Description

This documentation provides instructions for assembling and using the **YUXI ODOMETRY SET**.

This document **does not cover the mechanical assembly** of the odometry set. For mechanical assembly instructions, refer to the hardware documentation linked above. Instead, this document focuses on the electronic schematics, firmware, and software capabilities of the system.

Keep in mind that the electronics assembly affects some aspects of the mechanical assembly. Therefore, **make sure to read both the hardware and software documentation carefully before starting the assembly**.

## Assembly

### Bill of Materials

To assemble a complete odometry set consisting of the **Pinpoint Computer and two odometry pods**, you will need:

#### Electronic Components

* 1 × [Pimoroni Tiny 2350](https://arduino.ua/prod7127-plata-rozrobnika-pimoroni-tiny-2350-4mb)
* 1 × [LSM6DS3 IMU](https://arduino.ua/prod2775-modyl-akselerometra-ta-magnitometra-na-lsm6ds3)
* 2 × [AS5600 breakout boards](https://prom.ua/p1713901866-magnitnyj-enkoder-datchik.html)
* ~2 m of wire (AWG 18–26 is recommended, but you may use any suitable wire)
* Heat-shrink tubing in different sizes
* Perfboard
* Male header pins
* 3-pin and 4-pin male/female connectors (XH2.54 connectors are used in the example, but other suitable connectors may be used)
* 1 × 4-pin JST-PH connector, or another connector compatible with the I2C master device

#### Tools and Supplies

* 1 × USB Type-C cable for flashing the firmware
* Soldering station
* Solder and other supplies required for comfortable soldering

### Encoder Assembly

Once you have an **AS5600** breakout board (hereafter referred to as the **encoder**), perform the following steps before using it:

1. **Desolder the R4 resistor.**
2. Using a short piece of wire, connect the ***DIR*** pin to ***GND***.
3. Solder three identical wires, approximately **10–30 cm** long, to the ***VCC***, ***OUT***, and ***GND*** pins. Keep the solder joints as close to the breakout board as possible, with the wires facing toward the center of the board.
4. Place the breakout board into the **3D-printed case** and secure it with screws. Route the wires through the dedicated hole in the case.
5. Connect the wires to a **3-pin female connector**. The pinout used in the example is ***VCC–OUT–GND*** when viewed from the top side of the connector. Make sure to use heat-shrink tubing to insulate and secure the connections.
6. **Check all connections with a multimeter** before connecting the encoder to the rest of the system.

### Pinpoint Computer Assembly

1. Solder header pins to the **Pimoroni Tiny 2350** board (hereafter referred to as the **Tiny**) and the **LSM6DS3 IMU** (hereafter referred to as the **IMU**).
2. Solder the **Tiny** and **IMU** onto the perfboard. Make sure that **XYZ marking** on top of **IMU** is visible.
3. Prepare **two identical 4-wire 10–30 cm wires** with relatively large cross-section. These wires will be used for the ***VCC*** and ***GND*** connections.
4. Prepare approximately **7 pieces of thin wire**, each about **7 cm** long, and approximately **4 additional thin wires** that are **10–30 cm** long.
5. **Carefully follow the Tiny–IMU connection diagram below.** Use short, thin wires for these connections:

```text
Tiny        IMU

2  -------- SCL
3  -------- SDA
4  -------- SA0
5  -------- CS
```

6. Solder the four longer thin wires to the following Tiny pins. **Leave the other ends of these wires unconnected for now.**

```text
Tiny

A1  --------
A2  --------
0   --------
1   --------
```

7. Connect two of the free ends of the **first 5-wire cable** to ***VCC***: one to the ***VCC*** pin of the **Tiny** and one to the ***VCC*** pin of the **IMU**.
8. Connect two free ends of the **second 5-wire cable** to ***GND***: one to the ***GND*** pin of the **Tiny** and one to the ***GND*** pin of the **IMU**.
9. Connect the free signal and power wires to the connectors as follows:

```text
Male connector 1: VCC - A1 - GND
Male connector 2: VCC - A2 - GND
Connector 3: VCC - 0  - 1  - GND
```

**Make sure that the connector pinouts match the corresponding encoder and I2C master pinouts.** In particular:

* ***OUT*** from each encoder must be connected to the corresponding ***Ax*** pin.
* ***0*** and ***1*** must be connected to ***SDA*** and ***SCL*** of the I2C master, respectively.
* ***VCC*** and ***GND*** must be connected to the corresponding power and ground lines.

10. **Use heat-shrink tubing to insulate and secure all connections.**

11. Connect floating 3-male connectors to encoders, with "X" encoder being connected to A1 **Tiny** pin and "Y" encoder being connected to A2 **Tiny** pin. If, for some reason, you need to change **Tiny** pins, make sure to see corresponding section below on changing ***data.h*** file

### Placement on Robot

1. Position the **X and Y odometry pods** along perpendicular axes. Adjust their vertical positions to ensure good and consistent contact with the playing surface.
2. Mount the **Pinpoint Computer** so that the **upper side of the perfboard**, where the **Tiny** and **IMU** are mounted, faces the **top of the robot**. This orientation is **critical for the proper operation of the system**.

## Usage

### Overview

The **YUXI Odometry Set** is designed as an **I²C sensor** with a primary focus on:

* **High operational speed**
* **High precision**

The sensor is a **3.3 V-only device**. It communicates with the external master device over **I²C**. Internally, the Pinpoint Computer communicates with the IMU over **SPI** and reads the odometry encoders using its **ADC** inputs.

### Hardware Connection

Connect the YUXI Odometry Set to the I²C master as follows:

```text
Odometry    Master

VCC ------- 3V3
SDA ------- I2C SDA
SCL ------- I2C SCL
GND ------- GND
```

> **WARNING:** The YUXI Odometry Set is a **3.3 V-only device**. Connecting VCC to 5 V may damage the board.

### I²C Configuration

| Parameter  | Value     |
| ---------- | --------- |
| Address    | `0x69`    |
| Speed      | `100 kHz` |
| Addressing | `7-bit`   |

### Register Map

| Address | Name         | Size | Access | Type      | Description |
| ------- | ------------ | ---- | :----: | --------- | ----------- |
| `0x33`  | `RESET`      | —    | W      | Command   | Initiates a system reset and recalibration |
| `0x34`  | `X_ENABLED`  | 1 B  | W      | `bool`    | Enables/disables X-axis reading |
| `0x35`  | `Y_ENABLED`  | 1 B  | W      | `bool`    | Enables/disables Y-axis reading |
| `0x36`  | `G_ENABLED`  | 1 B  | W      | `bool`    | Enables/disables IMU reading |
| `0x45`  | `START_READ` | 1 B  | R      | `uint8_t` | Starting address for odometry data |
| `0x46`  | `X_POSITION` | 4 B  | R      | `float32` | X position |
| `0x4A`  | `Y_POSITION` | 4 B  | R      | `float32` | Y position |
| `0x4E`  | `HEADING`    | 4 B  | R      | `float32` | Robot heading |

#### Command Registers

Unlike regular writable registers, command registers do not store any value.

To execute the `RESET` command, the master only needs to send the `0x33` register address:

```text
START
  ↓
SLAVE ADDRESS + WRITE
  ↓
0x33
  ↓
STOP
```

#### Data Format

All `float32` values are encoded using the IEEE-754 single-precision
floating-point format and stored in **little-endian** byte order.

The complete odometry data occupies 12 consecutive bytes:

```text
0x46 ───── 0x49    X_POSITION
0x4A ───── 0x4D    Y_POSITION
0x4E ───── 0x51    HEADING