# Sample project for a RPM controller on a ATmega328P 

## Overview
This project is designed for the ATmega328P microcontroller. It includes functionalities such as RPM measurement, PWM control, UART communication, and a simple shell interface. The system utilizes a PI controller to maintain a target RPM for a motor.

## Features
- **RPM Measurement**: Calculates the rotations per minute of a motor using an interrupt-based approach.
- **PWM Control**: Provides a PWM signal to control motor speed.
- **UART Communication**: Handles serial communication for debugging and control.
- **Shell Interface**: A simple command-line interface for interacting with the system.
- **PI Controller**: Implements a Proportional-Integral controller to regulate motor speed.
