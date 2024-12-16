# EcoCar Sally Prototype
# H2 Sensor Board - Sally
***This is the official repository for the H2 Sensor Board of the 2024-2025 EcoCar prototype, Sally***

## Current revision: Rev 1.x (edit)

### Credits: Owen W., Abinash S., et al.

---

### Overview

Much of the systems of this vehicle will build on the innovations implemented in Lucy. Components and connectors were refreshed to better align with the Sally vehicle's electrical design.

The purpose of this board is to shut the system down if a certain preprogrammed concentration of H2 in the atmosphere.

It is a modular board, which uses a MQ-8 Winsen sensor on a detachable module thats held on a loft board. (edit)

We also included a OLED ssd1306 modular screen to display the measuremnts made by the sensor to debug and to enable live monitoring without serial. (edit)

It is CAN Enabled on CAN 2.0B and runs at 1Mbps. It can send out a broadcast message which can be used by the fuelcell controller and shut the car down immidiatly. (edit)

---

### Board Functional Breakdown (edit)

- Uses a MQ-8 Winsen hydrogen sensor
- AP62250 Diodes buck
- Uses 12V from fuel cell 
- Converts 12V to 5V 3V3
- Uses a STM32L474RET6
- BME280 atmosphere humidity pressure and temprature sensor
- Uses a I2C OLED screen to display data and current system state
- Uses 3 distinct RYG LED's to display system state.
- Addressable LED ring for external system state display
- USB-C onboard interface for programing and debugging
- Always ON life-sensing LED
- Functional hydrogen ALARM(beeper)

---

### Firmware
STM32 functionalized with HAL and FreeRTOS.

---

### Completed aspects


### Incomplete aspects



