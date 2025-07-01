# AVR ATmega2560 Communication & Control Projects

A comprehensive collection of AVR ATmega2560-based embedded projects integrating DC motors, stepper drivers, encoders, sensors, and multiple communication protocols. Includes demos and utilities for ADC, timers, SPI/UART, PS2 control, LCD interface, braking circuits, and more — all developed and tested in Atmel Studio.

## Development Environment

- **IDE:** Atmel Studio (latest version recommended)  
- **Microcontroller:** AVR ATmega2560  
- **Compiler:** AVR-GCC (bundled with Atmel Studio)  
- **Programmer:** ISP or compatible AVR programmer  
- **Clock Frequency:** 14.9456 MHz crystal oscillator (custom configured)  

---

## Supported Hardware & Components

| Component                 | Description                           | Links (Datasheets/Products)                    |
|---------------------------|-------------------------------------|------------------------------------------------------------|
| ATmega2560 MCU            | Main microcontroller                 | [ATmega2560 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega640-1280-1281-2560-2561_Datasheet.pdf) |
| Cytron Motor Driver       | DC motor driver                      | [Cytron Motor Driver](https://eu.robotshop.com/de/products/cytron-potentiometer-schaltersteuerung-10amp-dc-motor-driver?srsltid=AfmBOopvgcvHv9kNqUiQgqrpRmhXWerjc3NgNYDPC56cDvf13P7NfC7gG14) |
| A4988 Stepper Driver      | Stepper motor driver                 | [A4988 Stepper Drive](https://de.aliexpress.com/item/1005001621676770.html?af=2049356_1&cv=41155805&cn=46syopw9s87gsixiwdjt2q1n90nf2w55&dp=v5_46syopw9s87gsixiwdjt2q1n90nf2w55&af=2049356_1&cv=41155805&cn=46syopw9s87gsixiwdjt2q1n90nf2w55&dp=v5_46syopw9s87gsixiwdjt2q1n90nf2w55&utm_source=epn&utm_medium=cpa&utm_campaign=2049356_1&utm_content=41155805&product_id=1005001621676770&afref=&aff_fcid=fdf976ce2ee748c78c8b28746ca1b64e-1751313177294-03219-_9G57Xi&aff_fsk=_9G57Xi&aff_platform=portals-hotproduct&sk=_9G57Xi&aff_trace_key=fdf976ce2ee748c78c8b28746ca1b64e-1751313177294-03219-_9G57Xi&terminal_id=34e32124aa7c49db9ea7a9c91389e061&afSmartRedirect=y) |
| Rotary Encoder   | Incremental Rotary Encoder       | [Rotary Encoder](https://pankaj.com/wp-content/uploads/2016/03/NCP711-S_V.pdf) |
| DC Motor                  | Brushed DC motor                    | Controlled via PWM and direction pins                       |
| 16x2 LCD Display          | Alphanumeric LCD with HD44780       | [16x2 LCD](https://amzn.eu/d/g3ZN9aQ)                         |
| PS2 Controller            | Wireless game controller            | [Controller](https://amzn.eu/d/01R5jAL)                             |

---

## Project Overview

This repository contains modular code examples and libraries demonstrating:

- **ADC Reading & 10-bit to 8-bit Conversion:**  
  Smooth and fast ADC sampling with range conversion for compact data usage.
  ### 📂 Code  
  - [ADC](./01.%20ADC/ADC/main.c)

- **Pushbutton Debouncing and Digital Read:**  
  Reliable digital input reading with software debouncing logic.
  ### 📂 Code  
  - [Push_button](./02.%20Push_Button/button_module/main.c)  

- **Encoder Handling with Interrupts:**  
  Using external interrupts and timers to count rotary encoder pulses accurately.
  ### 📂 Code  
  - [Encoder_interrupt](./05.%20Drive_Encoder_Interrupt/Drive_Encoder_Interrupt/main.c)

- **Motor Control (DC & Stepper):**  
  PWM generation using Timer4 for speed control of DC motors and step/direction pulses for A4988 stepper driver.
  ### 📂 Code  
  - [DC_motor](./13.%20tb_DC_Motor/TEST%20MOTOR/main.c)
  - [Stepper_driver](./12.%20Stepper_a4988/Stepper_a4988/main.c)

- **LCD 16x2 Interface:**  
  Character display driven by HD44780-compatible code with easy-to-use functions.
  ### 📂 Code  
  - [Lcd_16x2](./07.%20LCD_16x2/LCD/main.c)  

- **SPI Communication:**  
  Full-duplex SPI master and slave modes demonstrated for inter-device communication.
  ### 📂 Code  
  - [SPI_slave](./09.%20SPI_SLAVE/SPI_SLAVE/main.c)
  - [SPI_master](./08.%20SPI_MASTER/SPI_MASTER/main.c)

- **UART Communication:**  
  Transmit and receive via UART at configurable baud rates with buffer management.
  ### 📂 Code  
  - [uart](./10.%20UART/UART/main.c)

- **PS2 Controller Input:**  
  Reading and parsing wireless PS2 controller signals to control motors or UI.
  ### 📂 Code  
  - [ps2_controller](./17.%20PS2/PS2/main.c)

- **Timer Compare and Overflow Usage:**  
  Using timer interrupts for precise delays, PWM duty cycle adjustment, and event counting.
  ### 📂 Code  
  - [Timer](./16.%20tb_Timer/timer_task/main.c)
  - [Timer_compare](./14.%20Timer_compare/Timer_compare/main.c)
  - [Timer_overflow](./15.%20Timer_Overflow/Timer_Overflow/main.c)

- **Braking Circuit Integration:**  
  Applying and releasing brake signals digitally for motor stop functionality.
  ### 📂 Code  
  - [breaking_circuit](./18.%20Breaking%20Circuit/Breaking%20Circuit/main.c)

---

## References

- Atmel Studio: [https://www.microchip.com/mplab/avr-support/atmel-studio-7](https://www.microchip.com/mplab/avr-support/atmel-studio-7)  
- ATmega2560 Datasheet (Microchip)  
- Cytron Motor Driver User Guide  
- A4988 Stepper Driver Datasheet  
- PS2 Controller Protocol Documentation  
- HD44780 LCD Controller Datasheet  

---

*Happy embedded designing and motor control!*