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
* 1 × USB Type-C cable for flashing the firmware

#### Tools and Supplies

* Soldering station
* Solder and other supplies required for comfortable soldering

### Encoder Assembly

Once you have an **AS5600** breakout board (hereafter referred to as the **encoder**), perform the following steps before using it:

1. **Desolder the R4 resistor.**
2. Using a short piece of wire, connect the ***DIR*** pin to ***GND***.
3. Solder three identical wires, approximately **10–30 cm** long, to the ***VCC***, ***OUT***, and ***GND*** pins. Keep the solder joints as close to the breakout board as possible, with the wires facing toward the center of the board.
4. Place the breakout board into the **3D-printed case** and secure it with screws. Route the wires through the dedicated hole in the case.
5. Connect the wires to a **3-pin female connector**. The pinout used in the YUXI Odometry Set is ***VCC–OUT–GND*** when viewed from the top side of the connector. Make sure to use heat-shrink tubing to insulate and secure the connections.
6. **Check all connections with a multimeter** before connecting the encoder to the rest of the system.

### Pinpoint Computer Assembly

1. Solder header pins to the **Pimoroni Tiny 2350** board (hereafter referred to as the **Tiny**) and the **LSM6DS3 IMU** (hereafter referred to as the **IMU**).
2. Solder the **Tiny** and **IMU** onto the perfboard.
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
