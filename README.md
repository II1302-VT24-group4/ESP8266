# 🚀 Welcome to the ESP8266 Code Repository!

This repository is includes all things related to the ESP8266 microcontroller.

## 🎯 Getting Started

Follow the guide below to set up and start programming NodeMCU ESP8266:

[🔗 Complete Guide to Setup and Program NodeMCU ESP8266](https://www.youtube.com/watch?v=YN522_npNqs)

## 📚 Essential Drivers

Make sure you have the necessary drivers installed. You can download them from the link below:

[🔗 USB to UART Bridge VCP Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads)

## 🤝 Contributing

We appreciate your interest in contributing to our project! Please read our Contributing Guidelines for information on how to contribute to this project.

## 📌 Display to NodeMCU Pin Connections

| Display Pin | Function              | NodeMCU Pin | Function       |
|-------------|-----------------------|-------------|----------------|
| 1 (SCL)     | Serial Clock input    | D5 (GPIO14) | SPI Clock      |
| 2 (SI)      | Serial Data input     | D7 (GPIO13) | SPI MOSI       |
| 3 (VDD)     | Supply Voltage (+3.0V)| 3V3         | 3.3V Power     |
| 4 (A0)      | Register Select       | D1 (GPIO5)  | GPIO           |
| 5 (/RESET)  | Reset signal          | D4 (GPIO2)  | GPIO           |
| 6 (/CS)     | Chip Select Signal    | D8 (GPIO15) | GPIO           |
| 7 (VSS)     | Ground                | GND         | Ground         |
| 8 (H)       | Heater Connection     | -           | Not Connected  |
| 9 (H)       | Heater Connection     | -           | Not Connected  |
| 10 (LED-)   | Backlight Cathode     | GND         | Ground         |
| 11 (LED+)   | Backlight Anode       | 3V3         | 3.3V Power     |
| 12 (NC)     | No Connect            | -           | Not Connected  |

## 📌 LEDs to NodeMCU Pin Connections

| NodeMCU Pin | Function       |
|-------------|----------------|
| D0 (GPIO16 )| Red led        |
| SD3 (GPIO9) | Green led      |

## 📌 Buzzer to NodeMCU Pin Connections

| NodeMCU Pin | Function       |
|-------------|----------------|
| D6 (GPIO12) | Sound          |

## 📌 Buttons to NodeMCU Pin Connections

| NodeMCU Pin | Function       |
|-------------|----------------|
| RX (GPIO3)  | Confirm button |
| TX (GPIO1)  | Abort button   |
| ADC0 (A0)   | Up, Down, Left, Right buttons |

## 📌 RFID to NodeMCU Pin Connections

| RFID Pin    | Function              | NodeMCU Pin |
|-------------|-----------------------|-------------|
| 1 (TX)      | TX                    | D3 (GPIO0)|
| 2 (RX)      | RX                    | D2 (GPIO4) |
| 3 (VCC)     | Supply Voltage (+5.0V)| VIN (+5.0V) |
| 4 (GND)     | Ground                | GND         |
