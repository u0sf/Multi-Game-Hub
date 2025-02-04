# Multi-Game Device for ESP32

## Overview
This project is a multi-game device powered by an ESP32 microcontroller and an OLED display. The device features an intuitive interface navigated using three buttons (Up, Down, Select) and includes three engaging games:

1. **Rock-Paper-Scissors**
2. **Guess the Number**
3. **The Catcher**

## Hardware Components
- **ESP32 Microcontroller**
- **OLED Display (SSD1306)**
- **Three Push Buttons** (Up, Down, Select)
- **Wires and Breadboard (optional for prototyping)**

## Pin Connections
- **OLED Display:**
  - VCC → 3.3V on ESP32
  - GND → GND
  - SCL → GPIO 22
  - SDA → GPIO 21

- **Buttons:**
  - Up Button → GPIO 23
  - Down Button → GPIO 4
  - Select Button → GPIO 0

## Features
- **Splash Screen:** Displays "Multi-Game Hub" for 2 seconds at startup.
- **Menu Navigation:** Easily switch between games using the Up, Down, and Select buttons.
- **Individual Games:** Each game has unique gameplay mechanics and score tracking (where applicable).

## How to Play
1. **Power on the Device:** The splash screen will appear.
2. **Navigate the Menu:** Use the Up and Down buttons to highlight a game, then press Select to start.
3. **Play the Game:** Follow the on-screen instructions for each game.
4. **Return to Menu:** Press the Select button during gameplay to return to the main menu.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/u0sf/Multi-Game-Hub.git
   ```
2. Open the `.ino` file using Arduino IDE.
3. Select the correct board (ESP32) and port.
4. Upload the code to the ESP32.

## License
This project is open-source and available under the MIT License.

