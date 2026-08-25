# YUXI ODOMETRY SOFTWARE DOCUMENTATION

## External References

* For **hardware documentation**, see the [YUXI Odometry Hardware repository](https://github.com/tacterex/yuxi-odometry-hardware).
* For the **FTC SDK library**, see: **TBA**
* For the **Legal FTC version**, see: **TBA**

## Short Description

This documentation provides instructions for assembling and using the **YUXI ODOMETRY SET**.

This document **does not cover the mechanical assembly** of the odometry set. For mechanical assembly instructions, refer to the hardware documentation linked above. Instead, this document focuses on the electronic schematics, firmware, and software capabilities of the system.

Keep in mind that the electronics assembly affects some aspects of the mechanical assembly. Therefore, **make sure to read both the hardware and software documentation carefully before starting the assembly**.

### Table of Contents

- [Assembly](#assembly)
  - [Bill of Materials](#bill-of-materials)
    - [Electronic Components](#electronic-components)
    - [Tools and Supplies](#tools-and-supplies)
  - [Encoder Assembly](#encoder-assembly)
  - [Pinpoint Computer Assembly](#pinpoint-computer-assembly)
  - [Placement on Robot](#placement-on-robot)

- [Firmware Installation](#firmware-installation)
  - [Requirements](#requirements)
  - [Building the Firmware](#building-the-firmware)
  - [Flashing the Firmware](#flashing-the-firmware)
  - [Verifying the Installation](#verifying-the-installation)

- [Usage](#usage)
  - [Quick Start](#quick-start)
  - [Overview](#overview)
  - [Hardware Connection](#hardware-connection)
  - [I²C Configuration](#i2c-configuration)
  - [Register Map](#register-map)
  - [Data Encoding](#data-encoding)
  - [I²C Transactions](#i2c-transactions)
    - [Reading Odometry Data](#reading-odometry-data)
    - [Sending Commands](#sending-commands)
  - [Initialization](#initialization)
  - [LED Status Indicators](#led-status-indicators)
    - [Sensor Startup](#sensor-startup)
    - [RESET Command](#reset-command)
    - [LED State Reference](#led-state-reference)
  - [Technical Specifications](#technical-specifications)

- [Firmware Structure](#firmware-structure)
  - [Firmware Development](#firmware-development)
  - [Modifying the Firmware](#modifying-the-firmware)
    - [Hardware Configuration](#hardware-configuration)
    - [I²C Configuration](#i2c-configuration-1)
    - [SPI / IMU Configuration](#spi--imu-configuration)
    - [Mechanical Configuration](#mechanical-configuration)
    - [IMU Configuration](#imu-configuration)
    - [I²C Slave Registers](#i2c-slave-registers)
    - [What Should Usually Be Changed?](#what-should-usually-be-changed)

- [MicroPython Examples](#micropython-examples)
  - [Example Overview](#example-overview)
  - [Requirements](#requirements-1)
  - [Running the Examples](#running-the-examples)

- [Recommended Integration Workflow](#recommended-integration-workflow)

- [Troubleshooting](#troubleshooting)
  - [Device is not detected on I²C](#device-is-not-detected-on-i2c)
  - [Device is detected, but reading data times out](#device-is-detected-but-reading-data-times-out)
  - [Odometry values are incorrect](#odometry-values-are-incorrect)
  - [Heading is incorrect after reset](#heading-is-incorrect-after-reset)
  - [Sensor does not reach solid yellow](#sensor-does-not-reach-solid-yellow)
  - [Odometry scale is incorrect](#odometry-scale-is-incorrect)
  - [Communication works but values are nonsensical](#communication-works-but-values-are-nonsensical)

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

11. Connect floating 3-male connectors to encoders, with "X" encoder being connected to A1 **Tiny** pin and "Y" encoder being connected to A2 **Tiny** pin. If, for some reason, you need to change **Tiny** pins, make sure to visit corresponding section below on changing ***data.h*** file. **Only make changes when you are exactly sure about the efect of changes**..

### Placement on Robot

1. Position the **X and Y odometry pods** along perpendicular axes. Adjust their vertical positions to ensure good and consistent contact with the playing surface.
2. Mount the **Pinpoint Computer** so that the **upper side of the perfboard**, where the **Tiny** and **IMU** are mounted, faces the **top of the robot**. This orientation is **critical for the proper operation of the system**.

## Firmware Installation

The YUXI Odometry firmware must be installed on the **Pimoroni Tiny 2350** before the Pinpoint Computer can be used.

### Requirements

The firmware build requires:

* A computer with Git installed
* Visual Studio Code with Raspberry Pico plugin
* Raspberry Pi Pico SDK and its required toolchain
* The YUXI firmware repository
* A USB Type-C cable
* A Pimoroni Tiny 2350

### Building the firmware

Clone or download the YUXI Odometry firmware repository and open the firmware directory in Visual Studio Code.

Make sure the project is configured for the **Pimoroni Tiny 2350** board and generate the CMake build files.

Build the project using the `'Compile'` button in the window

### Flashing the firmware

1. Disconnect the Tiny 2350 from the robot.
2. Connect the Tiny 2350 to the computer using USB.
3. Enter the RP2350 bootloader mode according to the Pimoroni Tiny 2350 documentation.
4. The board should appear as a USB mass-storage device.
5. Click `'Run'`.
6. Wait for the board to reboot automatically.
7. Disconnect and reconnect the USB cable if necessary.

After flashing, the firmware will start automatically.

### Verifying the Installation

After flashing the firmware, keep the **Pimoroni Tiny 2350 connected to the PC via USB**. There is no need to connect the YUXI Odometry Set to an I²C master at this stage.

After the board reboots, observe the status LED and verify that it progresses through the normal startup sequence and eventually becomes **solid yellow**.

A **solid yellow LED** indicates that the firmware has successfully started and completed its initialization process.

If the LED does not reach the normal-operation state, or the board does not behave as expected after flashing, see the **Troubleshooting** section.

## Usage

### Quick Start

This section provides the shortest procedure for getting a fully assembled YUXI Odometry Set working.

#### 1. Connect the sensor

Connect the YUXI Odometry Set to the I²C master:

```text
YUXI        I²C Master

VCC ------- 3V3
SDA ------- SDA
SCL ------- SCL
GND ------- GND
```

The YUXI Odometry Set operates at **3.3 V**. Do not connect VCC to 5 V.

#### 2. Verify the I²C connection

Configure the master for:

| Parameter   | Value     |
| ----------- | --------- |
| I²C address | `0x69`    |
| I²C speed   | `100 kHz` |
| Addressing  | `7-bit`   |

Scan the I²C bus and verify that device `0x69` is detected.

#### 3. Power on the YUXI system

After powering the system, wait until the status LED becomes **solid yellow**.

During the blue LED stage, keep the robot completely stationary because the IMU is being calibrated.

#### 4. Reset the odometry

After the I²C connection has been established, send the `RESET` command:

```python
ADDR = 0x69
RESET = 0x33

i2c.writeto(ADDR, bytes([RESET]))
```

Keep the robot stationary until the initialization and calibration process is complete.

The `RESET` command establishes the current robot position and heading as the new `(0, 0, 0)` reference.

#### 5. Read odometry data

Request 12 bytes starting from register `0x45`:

```python
START_REGISTER = 0x45

i2c.writeto(ADDR, bytes([START_REGISTER]))
data = i2c.readfrom(ADDR, 12)
```

Decode the returned data as three little-endian `float32` values:

```python
import struct

pos_x, pos_y, heading = struct.unpack("<fff", data)

print(f"X: {pos_x:.2f} mm")
print(f"Y: {pos_y:.2f} mm")
print(f"Heading: {heading:.4f} rad")
```

The returned values are:

```text
POS_X   — X position in millimeters
POS_Y   — Y position in millimeters
HEADING — heading in radians
```

The system is now ready to be integrated into the robot control software.

### Overview

The **YUXI Odometry Set** is designed as an **I²C sensor** with a primary focus on:

* **High operational speed**
* **High precision**

Internally, the Pinpoint Computer communicates with the IMU over **SPI** and reads the odometry encoders using its **ADC** inputs.

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

| Parameter        | Value      |
| ---------------- | ---------- |
| Address          | `0x69`     |
| Speed            | `100 kHz`  |
| Addressing       | `7-bit`    |
| Data update time | `< 1ms`    |
| Data buffer size | `12 bytes` |

### Register Map

| Address | Name         | Size | Access | Type      | Description |
| ------- | ------------ | ---- | :----: | --------- | ----------- |
| `0x33`  | `RESET`      | —    | W      | Command   | Initiates a system reset and recalibration |
| `0x34`  | `X_ENABLED`  | 1 B  | W      | `bool`    | Enables/disables X-axis reading |
| `0x35`  | `Y_ENABLED`  | 1 B  | W      | `bool`    | Enables/disables Y-axis reading |
| `0x36`  | `G_ENABLED`  | 1 B  | W      | `bool`    | Enables/disables IMU reading |
| `0x45`  | `X_POSITION` | 4 B  | R      | `float32` | X position |
| `0x49`  | `Y_POSITION` | 4 B  | R      | `float32` | Y position |
| `0x4D`  | `HEADING`    | 4 B  | R      | `float32` | Robot heading |

---

#### RESET

| Address | Size | Access | Type    |
| ------- | ---- | :----: | ------- |
| `0x33`  | —    |    W   | Command |

The `RESET` command performs a full system reinitialization. It:

* Reinitializes the ADC.
* Reinitializes the SPI interface.
* Recalibrates the IMU.
* Resets the odometry position to `0`.
* Resets the gyro angle to `0`.

The `RESET` command **does not require or store a data value**. To execute it, the master only needs to send the `0x33` address. No data byte should follow the address.

Example transaction:

```text
START
  ↓
SLAVE ADDRESS + WRITE
  ↓
0x33
  ↓
STOP
```

The odometry data output **continues during the reinitialization process**. Once initialization is complete, the position and heading are reset to `0` and normal operation resumes.

Other configuration registers are **not affected** by the `RESET` command.

It is recommended to execute `RESET` after restarting the master program to ensure that the odometry system is properly initialized and synchronized with the master.

---

#### X_ENABLED

| Address | Size | Access | Type   |
| ------- | ---- | :----: | ------ |
| `0x34`  | 1 B  |    W   | `bool` |

Enables or disables X-axis odometry calculations.

| Value              | Action          |
| ------------------ | --------------- |
| `0x00`             | X-axis enabled  |
| Any non-zero value | X-axis disabled |

The value is interpreted as follows:

```text
0x00      → enabled
0x01–0xFF → disabled
```

---

#### Y_ENABLED

| Address | Size | Access | Type   |
| ------- | ---- | :----: | ------ |
| `0x35`  | 1 B  |    W   | `bool` |

Enables or disables Y-axis odometry calculations.

| Value              | Action          |
| ------------------ | --------------- |
| `0x00`             | Y-axis enabled  |
| Any non-zero value | Y-axis disabled |

The value is interpreted as follows:

```text
0x00      → enabled
0x01–0xFF → disabled
```

---

#### G_ENABLED

| Address | Size | Access | Type   |
| ------- | ---- | :----: | ------ |
| `0x36`  | 1 B  |    W   | `bool` |

Enables or disables gyro readings.

| Value              | Action        |
| ------------------ | ------------- |
| `0x00`             | Gyro enabled  |
| Any non-zero value | Gyro disabled |

The value is interpreted as follows:

```text
0x00      → enabled
0x01–0xFF → disabled
```

> **Important:** Disabling the gyro does **not** reset its angle or recalibrate the IMU. It only stops gyro readings from being used by the odometry system. To reset the gyro angle and perform IMU calibration, execute the `RESET` command.

---

#### POS_X

| Address     | Size | Access | Type      | Unit |
| ----------- | ---- | :----: | --------- | ---- |
| `0x45–0x48` | 4 B  |    R   | `float32` | mm   |

Contains the current **X-axis position** calculated by the odometry system **relative to the position and heading at which the `RESET` command was last executed**.

The value is encoded as an **IEEE-754 single-precision floating-point number** in **little-endian** byte order.

> **Important:** Disabling the X-axis using `X_ENABLED` does **not** reset or otherwise modify this register. The position is continuously calculated relative to the starting position established by the most recent `RESET` command.
>
> However, disabling the X-axis reading may improve performance or precision in some simpler drivetrain configurations, such as **tank drivetrains**. Since the odometry system accounts for robot rotation, the calculated X position can be affected by **Y-axis movement when the robot rotates**. Therefore, both axis readings may be required for accurate X-position tracking in systems where the robot can freely rotate.

---

#### POS_Y

| Address     | Size | Access | Type      | Unit |
| ----------- | ---- | :----: | --------- | ---- |
| `0x49–0x4C` | 4 B  |    R   | `float32` | mm   |

Contains the current **Y-axis position** calculated by the odometry system **relative to the position and heading at which the `RESET` command was last executed**.

The value is encoded as an **IEEE-754 single-precision floating-point number** in **little-endian** byte order.

> **Important:** Disabling the Y-axis using `Y_ENABLED` does **not** reset or otherwise modify this register. The position is continuously calculated relative to the starting position established by the most recent `RESET` command.
>
> However, disabling the Y-axis reading may improve performance or precision in some simpler drivetrain configurations, such as **tank drivetrains**. Since the odometry system accounts for robot rotation, the calculated Y position can be affected by **X-axis movement when the robot rotates**. Therefore, both axis readings may be required for accurate Y-position tracking in systems where the robot can freely rotate.

---

#### HEADING

| Address     | Size | Access | Type      | Unit |
| ----------- | ---- | :----: | --------- | ---- |
| `0x4D–0x50` | 4 B  |    R   | `float32` | rad  |

Contains the current **robot heading** calculated using the IMU gyro.

The heading is measured **relative to the orientation at which the `RESET` command was last executed**. Immediately after a successful reset and IMU calibration, the heading is set to `0`.

The value is encoded as an **IEEE-754 single-precision floating-point number** in **little-endian** byte order.

> **Important:** Disabling the gyro using `G_ENABLED` does **not** reset the heading or recalibrate the IMU. It only disables gyro readings from being used by the odometry calculation.
>
> To reset the heading and recalibrate the IMU, execute the `RESET` command (`0x33`).

---

### Data Encoding

All `float32` values are encoded using the **IEEE-754 single-precision floating-point format** and transmitted in **little-endian** byte order.

The three odometry values occupy **12 consecutive bytes**, starting from register `0x46`:

| Bytes  | Register  | Type      | Unit |
| ------ | --------- | --------- | ---- |
| `0–3`  | `POS_X`   | `float32` | mm   |
| `4–7`  | `POS_Y`   | `float32` | mm   |
| `8–11` | `HEADING` | `float32` | rad  |

The corresponding register ranges are:

```text
0x45 ───── 0x48    POS_X
0x49 ───── 0x4C    POS_Y
0x4D ───── 0x50    HEADING
```

When reading all three values at once, the master should request **12 bytes starting from register `0x45`**.

### I²C Transactions

#### Reading Odometry Data

To read the current odometry data, the master should first specify `0x45` as the starting register and then request **12 bytes**.

The transaction is:

```text
START
  ↓
SLAVE ADDRESS + WRITE
  ↓
0x45
  ↓
REPEATED START
  ↓
SLAVE ADDRESS + READ
  ↓
12 BYTES
  ↓
STOP
```

The returned data is organized as follows:

```text
Byte  0 ─────  3    POS_X
Byte  4 ─────  7    POS_Y
Byte  8 ───── 11    HEADING
```

Each value is encoded as an **IEEE-754 `float32`** in **little-endian** byte order.

Using MicroPython:

```python
ADDR = 0x69
START_REGISTER = 0x45

i2c.writeto(ADDR, bytes([START_REGISTER]))
data = i2c.readfrom(ADDR, 12)
```

The returned `data` object contains all three values as 12 consecutive bytes.

The values can be decoded using Python's `struct` module:

```python
import struct

pos_x, pos_y, heading = struct.unpack("<fff", data)

print(f"X: {pos_x:.2f} mm")
print(f"Y: {pos_y:.2f} mm")
print(f"Heading: {heading:.4f} rad")
```

The `<fff` format string specifies:

* `<` — little-endian byte order
* First `f` — `POS_X`
* Second `f` — `POS_Y`
* Third `f` — `HEADING`

#### Sending Commands

Commands are sent by writing the corresponding command address to the YUXI I²C device.

For example, to execute `RESET`, the master should send only the `0x33` register address:

```text
START
  ↓
SLAVE ADDRESS + WRITE
  ↓
0x33
  ↓
STOP
```

No data byte should be sent after `0x33`.

In MicroPython:

```python
ADDR = 0x69
RESET = 0x33

i2c.writeto(ADDR, bytes([RESET]))
```

The `RESET` command is executed when the device receives the `0x33` address.

For configuration registers such as `X_ENABLED`, `Y_ENABLED`, and `G_ENABLED`, the master must send both the register address and the desired value.

For example, to disable X-axis readings:

```python
ADDR = 0x69
X_ENABLED = 0x34

i2c.writeto(ADDR, bytes([X_ENABLED, 0x01]))
```

To enable X-axis readings again:

```python
i2c.writeto(ADDR, bytes([X_ENABLED, 0x00]))
```

### Initialization

The YUXI Odometry Set should be initialized after the I²C master starts communication with the device.

The recommended initialization sequence is:

1. Initialize the I²C master using the configuration specified in the **I²C Configuration** section.
2. Detect or connect to the YUXI Odometry Set at address `0x69`.
3. Send the `RESET` command (`0x33`).
4. Allow the YUXI system to complete its initialization and sensor calibration.
5. Start reading the odometry data from register `0x46`.

The `RESET` command establishes the initial reference position and heading for the odometry system. All subsequent `POS_X`, `POS_Y`, and `HEADING` values are calculated relative to this reference.

> **Important:** It is recommended to execute `RESET` whenever the master program is restarted. This ensures that the odometry system establishes a new reference position corresponding to the current state of the robot.

---

### LED Status Indicators

The YUXI Odometry Sensor uses dedicated LED signals to indicate its current operating state. These indicators are especially useful during startup, initialization, calibration, and troubleshooting.

The LED states are summarized below:

| LED State                 | Meaning                                           | Typical Duration |
| ------------------------- | ------------------------------------------------- | ---------------- |
| **⚪White (solid)**         | Sensor is powered on                              | —                |
| **🔵Blue (solid)**          | IMU calibration and hardware initialization       | ~1 s             |
| **🟡Yellow (single blink)** | I²C slave behavior initialization                 | ~0.5 s           |
| **🟡Yellow (solid)**        | Normal operation; sensor is ready to provide data | Continuous       |

#### Sensor Startup

The startup sequence is as follows:

```text
┌──────────────────────────────────────────────┐
│ Sensor powered on                            │
│ LED: WHITE (solid)                           │
└──────────────────────────────────────────────┘
                     │
                     │ < 0.1 s
                     ▼
┌──────────────────────────────────────────────┐
│ Hardware initialization & IMU calibration    │
│ LED: BLUE (solid)                            │
└──────────────────────────────────────────────┘
                     │
                     │ ~1 s
                     ▼
┌──────────────────────────────────────────────┐
│ I²C slave behavior initialization            │
│ LED: YELLOW (single blink)                   │
└──────────────────────────────────────────────┘
                     │
                     │ ~0.5 s
                     ▼
┌──────────────────────────────────────────────┐
│ Normal operation                             │
│ LED: YELLOW (solid)                          │
└──────────────────────────────────────────────┘
```

Once the LED becomes **solid yellow**, the sensor has completed its initialization and is ready for normal operation.

#### RESET Command

Executing the `RESET` command triggers the hardware initialization and IMU calibration sequence again.

The reset sequence is:

```text
┌──────────────────────────────────────────────┐
│ Normal operation                             │
│ LED: YELLOW (solid)                          │
└──────────────────────────────────────────────┘
                     │
                     │ < 0.1 s
                     ▼
┌──────────────────────────────────────────────┐
│ Hardware initialization & IMU calibration    │
│ LED: BLUE (solid)                            │
└──────────────────────────────────────────────┘
                     │
                     │ ~1 s
                     ▼
┌──────────────────────────────────────────────┐
│ Normal operation                             │
│ LED: YELLOW (solid)                          │
└──────────────────────────────────────────────┘
```

> **⚠️ IMPORTANT:** The robot **must remain completely stationary while the LED is solid blue**. During this stage, the IMU is calibrating and establishing the stationary reference used for angle calculation. Moving the robot during calibration can result in an incorrect heading and negatively affect subsequent odometry calculations.

### LED State Reference

For quick troubleshooting, the LED states can be interpreted as follows:

* ⚪ **White (solid)** — Power is present; initialization has not started yet.
* 🔵 **Blue (solid)** — Hardware is being initialized and the IMU is calibrating. **Keep the robot stationary.**
* 🟡 **Yellow (single blink)** — I²C slave behavior is being initialized.
* 🟡 **Yellow (solid)** — Initialization is complete and the sensor is operating normally.

### Technical Specifications

The following parameters may be useful when integrating the YUXI Odometry Sensor with other systems:
 
| Parameter              | Specification        |
| ---------------------- | -------------------- |
| I²C slave address      | `0x69`               |
| I²C bus speed          | `100 kHz`            |
| I²C addressing         | `7-bit`              |
| Supply voltage         | `3.3 V`              |
| Internal SPI baud rate | `2 MHz`              |
| ADC resolution         | `12-bit`             |
| Position data type     | `IEEE-754 float32`   |
| Position units         | `mm`                 |
| Heading units          | `rad`                |
| Odometry data size     | `12 bytes`           |
| IMU                    | `LSM6DS3`            |
| Encoder                | `AS5600` ×2          |
| Head controller        | `Pimoroni Tiny 2350` |

## Firmware Structure

The YUXI Odometry firmware is organized into separate modules for data management, hardware peripherals, odometry processing, I²C communication and testing. Firmware is written using PICO SDK

| Path / File | Description |
| ----------- | ----------- |
| `headers/` | Header files for shared data, peripherals, and processes |
| `headers/data.h` | Shared data definitions |
| `headers/peripherals.h` | Peripheral interfaces declaration|
| `headers/processes.h` | Processes (position calculation and slave behaivour) declaration |
| `peripherals/` | Modules responsible for hardware interaction |
| `peripherals/EncoderReader.cpp` | AS5600 encoder and ADC management implementation |
| `peripherals/IMUReader.cpp` | LSM6DS3 IMU and SPI interface implementation |
| `peripherals/LedsManager.cpp` | Status LED control implementation|
| `processes/` | Modules implementing the main firmware processes |
| `processes/I2CSlave.cpp` | I²C slave communication and register handling implementation|
| `processes/PositionCalculator.cpp` | Odometry position and heading calculation implementation|
| `data.cpp` | Shared data implementation |
| `main.cpp` | Firmware entry point and high-level initialization |
| `CMakeLists.txt` | CMake build configuration |
| `pico_sdk_import.cmake` | Raspberry Pi Pico SDK integration |
| `README.md` | Project documentation |

### Firmware Development

The YUXI Odometry firmware is written in **C++** using the **Raspberry Pi Pico SDK** and is built using **CMake**.

The firmware targets the **Pimoroni Tiny 2350 (RP2350)**.

The project is organized so that hardware configuration and communication parameters can be modified independently from the main implementation. For most hardware customizations, the primary configuration file is:

```text
headers/data.h
```

See **Modifying the Firmware** for a description of the parameters that can safely be changed.

When modifying the firmware, make sure that changes to the hardware configuration remain consistent with the physical wiring and that changes to the I²C protocol are reflected in the corresponding master-side software.

### Modifying the Firmware

Most hardware-specific and communication-related parameters are centralized in `headers/data.h`. This allows common configuration values to be changed without modifying the implementation of individual modules. Other files aren't dedicated for modifying

> **Important:** Only modify the parameters described below if you understand their effect on the corresponding hardware or communication interface. Incorrect changes to GPIO assignments or protocol parameters may prevent the firmware from operating correctly.

#### Hardware Configuration

The `hardware` namespace contains parameters related to GPIO assignments and hardware interfaces.

| Parameter | Default | Description |
| --------- | ------- | ----------- |
| `adc0_gpio` | `26` | GPIO used for the ADC encoder input |
| `encoder_max_value` | `4095` | Maximum value of the 12-bit ADC |
| `imu_max_value` | `65535` | Maximum value of the 16-bit IMU measurement range |
| `imu_half_max_value` | `32768` | Half of the 16-bit IMU measurement range |
| `LED_RED_GPIO` | `18` | GPIO connected to the red LED channel |
| `LED_GREEN_GPIO` | `19` | GPIO connected to the green LED channel |
| `LED_BLUE_GPIO` | `20` | GPIO connected to the blue LED channel |

If the hardware is modified to use different GPIOs, the corresponding definitions can be changed here.

#### I²C Configuration

The following parameters configure the I²C slave interface:

| Parameter | Default | Description |
| --------- | ------- | ----------- |
| `_i2c_i` | `0` | I²C peripheral instance |
| `_i2c_sda` | `0` | SDA GPIO |
| `_i2c_scl` | `1` | SCL GPIO |
| `_i2c_baudrate` | `100000` | I²C bus speed in Hz |
| `_i2c_n_nw_regs` | `1` | Number of non-write registers/commands |
| `_i2c_n_w_regs` | `3` | Number of writable registers |

The I²C slave address and register addresses are defined separately in the `_i2c_slave_registers` enum.

#### SPI / IMU Configuration

The SPI parameters define the connection between the Tiny 2350 and the LSM6DS3 IMU:

| Parameter | Default | Description |
| --------- | ------- | ----------- |
| `_spi_i` | `0` | SPI peripheral instance |
| `_spi_scl` | `2` | SPI clock GPIO |
| `_spi_miso` | `3` | SPI MISO GPIO |
| `_spi_mosi` | `4` | SPI MOSI GPIO |
| `_spi_cs_pin` | `5` | IMU chip-select GPIO |
| `_spi_baudrate` | `2000000` | SPI clock speed in Hz |
| `_g_cali_samples` | `1200` | Number of samples used for gyro calibration |

The number of calibration samples can be adjusted if a different calibration duration is required.

> **Important:** The robot must remain stationary while the IMU is being calibrated.

#### Mechanical Configuration

The `mechanical` namespace contains parameters describing the physical odometry hardware.

| Parameter | Default | Description |
| --------- | ------- | ----------- |
| `WHEEL_DIAMETER_MM` | `50` | Effective odometry wheel diameter in millimeters |

If odometry wheels with a different diameter are used, this value must be changed accordingly.

> **Important:** The wheel diameter directly affects the conversion from encoder measurements to physical displacement. An incorrect value will result in incorrect position calculations.

#### IMU Configuration

The `_imu_registers` enum contains the LSM6DS3 register addresses and configuration values used by the firmware.

| Parameter | Default | Description |
| --------- | ------- | ----------- |
| `WHO_AM_I` | `0x0F` | IMU identification register |
| `CTRL2_G` | `0x11` | Gyroscope configuration register |
| `OUTZ_L_G` | `0x26` | Gyroscope Z-axis low-byte register |
| `OUTZ_H_G` | `0x27` | Gyroscope Z-axis high-byte register |
| `ODR_G_104HZ` | `104 Hz` | Gyroscope output data rate |
| `FS_G_500DPS` | `±500 dps` | Gyroscope full-scale range |

The register addresses are defined by the LSM6DS3 datasheet and should normally not be changed. Configuration values such as the output data rate and full-scale range can be changed if the corresponding firmware calculations are also adjusted.

#### I²C Slave Registers

The `_i2c_slave_registers` enum defines the YUXI I²C communication protocol.

The I²C slave address can be changed here:

| Parameter | Default | Description |
| --------- | ------- | ----------- |
| `ADDRESS` | `0x69` | I²C slave address |

The command and data registers are also defined in this enum:

| Register | Address | Description |
| -------- | ------- | ----------- |
| `RESET_NW` | `0x33` | Executes system reset and recalibration |
| `X_ENABLED` | `0x34` | Enables/disables X-axis reading |
| `Y_ENABLED` | `0x35` | Enables/disables Y-axis reading |
| `G_ENABLED` | `0x36` | Enables/disables gyro reading |
| `START_READ` | `0x45` | Starting register for odometry data |
| `READ_1` – `READ_11` | `0x46` – `0x50` | Consecutive bytes of odometry data |

> **Important:** I²C register addresses are part of the YUXI communication protocol. Changing them makes existing master-side software incompatible with the modified firmware. Only change them if the corresponding master implementation is also modified.

#### What Should Usually Be Changed?

For a standard YUXI hardware build, **no changes to `data.h` should be required**.

The most common reasons to modify this file are:

- Using different GPIO connections.
- Using odometry wheels with a different diameter.
- Changing the I²C slave address.
- Changing I²C or SPI communication speed.
- Changing the IMU configuration.
- Adjusting the gyro calibration parameters.
- Adapting the firmware to a modified hardware revision.

Other firmware files generally do not need to be modified for basic hardware customization.

### MicroPython Examples

The `examples/micropython/` directory contains standalone MicroPython scripts demonstrating basic communication with the YUXI Odometry Sensor.

All examples assume the following I²C configuration:

| Parameter | Value |
| --------- | ----- |
| I²C address | `0x69` |
| I²C speed | `100 kHz` |
| SDA | GPIO `0` |
| SCL | GPIO `1` |

> **Note:** The GPIO numbers above refer to the **master device** running MicroPython. They do not need to match the GPIO configuration of the YUXI sensor.

#### Example Overview

| File | Description |
| ---- | ----------- |
| `check_connection.py` | Checks whether YUXI is detected on the I²C bus |
| `read_data.py` | Continuously reads X position, Y position, and heading |
| `reset_command.py` | Sends the `RESET` command and recalibrates the sensor |
| `disable_sensors.py` | Enables or disables X, Y, and gyro measurements |
| `full_test.py` | Complete example combining connection check, reset, and odometry reading |

#### Requirements

The examples require a MicroPython-compatible master device with:

- I²C master support
- SDA and SCL pins available
- MicroPython firmware installed

Only standard MicroPython modules are used, so **no external libraries are required**.

#### Running the Examples

Copy the required `.py` file from `examples/micropython/` to the MicroPython master device and execute it.

The examples are intended to be used as a starting point for integration. They can be modified and combined with the user's robot-control code as required.

### Recommended Integration Workflow

When integrating YUXI into a robot control system, the following sequence is recommended:

1. **Verify hardware**
   Confirm the power, ground, SDA, and SCL connections.

2. **Verify I²C communication**
   Scan the bus and confirm that `0x69` is detected.

3. **Test reset**
   Send the `RESET` command and verify that the sensor completes calibration.

4. **Test raw odometry output**
   Read the 12-byte data block and decode the three `float32` values.

5. **Verify stationary behavior**
   With the robot stationary, confirm that the position and heading remain approximately constant.

6. **Verify each axis independently**
   Move the robot along the X and Y directions and confirm that the corresponding position changes as expected.

7. **Verify heading**
   Rotate the robot and confirm that the heading changes in the expected direction.

8. **Integrate with the robot control system**
   Once the sensor output has been verified independently, integrate the readings into the robot's localization or motion-control software.

This approach separates hardware and communication problems from robot-control software problems and makes troubleshooting easier.

## Troubleshooting

This section lists the most common problems encountered when installing or integrating the YUXI Odometry Set.

### Device is not detected on I²C

If an I²C scan does not detect the YUXI device at `0x69`, check the following:

1. Verify that the sensor is powered from **3.3 V**.
2. Verify that **GND is connected** between the sensor and the I²C master.
3. Check that SDA and SCL are connected to the correct pins.
4. Verify that the master is using **7-bit address `0x69`**.
5. Verify that the I²C bus is configured for **100 kHz**.
6. Check all solder joints and connectors.
7. Verify that the status LED reaches the normal-operation state.

Do not continue troubleshooting the software protocol until the device is detected by an I²C scan.

### Device is detected, but reading data times out

If the device is detected but an I²C read fails or times out:

1. Verify that the master first writes the starting register `0x45`.
2. Verify that the master requests exactly **12 bytes** for a complete odometry reading.
3. Check that the I²C bus speed is `100 kHz`.
4. Check the SDA and SCL wiring.
5. Verify that the sensor firmware is running normally and the status LED is solid yellow.

A correct read transaction is:

```text
START
  ↓
SLAVE ADDRESS + WRITE
  ↓
0x45
  ↓
REPEATED START
  ↓
SLAVE ADDRESS + READ
  ↓
12 BYTES
  ↓
STOP
```

### Odometry values are incorrect

If the device communicates correctly but the position or heading is incorrect:

1. Verify that both odometry pods are mounted correctly.
2. Verify that the X and Y pods are positioned along perpendicular axes.
3. Check the orientation of the Pinpoint Computer.
4. Verify the configured odometry wheel diameter.
5. Execute `RESET` while the robot is stationary.
6. Keep the robot completely stationary during IMU calibration.
7. Check that the X encoder is connected to the configured X input and the Y encoder is connected to the configured Y input.

### Heading is incorrect after reset

If the heading is incorrect immediately after reset:

1. Place the robot on a stable surface.
2. Ensure that the robot does not move during IMU calibration.
3. Execute `RESET` again.
4. Wait until the status LED returns to solid yellow before moving the robot.

Movement during calibration can result in an incorrect stationary reference.

### Sensor does not reach solid yellow

The normal startup sequence is:

```text
WHITE  →  BLUE  →  YELLOW BLINK  →  YELLOW SOLID
```

If the sensor remains in an earlier state, check:

* Power supply
* Tiny 2350 connections
* IMU connections
* Firmware installation
* Firmware configuration in `data.h`

If the problem persists, inspect the firmware serial/debug output if available.

### Odometry scale is incorrect

If the robot moves a known distance but the reported position has an incorrect scale, check:

```cpp
WHEEL_DIAMETER_MM
```

in `headers/data.h`.

The configured value must correspond to the effective diameter of the odometry wheel used by the robot.

### Communication works but values are nonsensical

Verify that the received 12 bytes are decoded as:

```text
float32
little-endian
```

using:

```python
struct.unpack("<fff", data)
```

The three values are ordered as:

```text
POS_X
POS_Y
HEADING
```

Do not interpret the returned bytes as integers.