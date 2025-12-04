# Arduino Simon Says Game 🎮

A classic memory game implemented using an Arduino Uno, featuring an I2C LCD display, custom difficulty levels, and audio feedback.

**[View Demo on LinkedIn](https://www.linkedin.com/posts/malhar-salunkhe_mini-project-simon-says-game-combined-ugcPost-7343214441007607808-8K5Y?utm_source=share&utm_medium=member_desktop&rcm=ACoAAFJHFE4Brztz-3eARJygA53XmpjYMaQKBHw)**

## 📖 Overview

This project is an electronic implementation of the popular "Simon Says" game. The system generates a sequence of tones and lights that the player must repeat. As the game progresses, the sequences become longer and faster. This version includes a visual interface (LCD) to select difficulty levels and track scores.

## ✨ Features

* **4 Difficulty Levels:** Easy, Medium, Hard, and Insane.
* **Visual Feedback:** 16x2 LCD Display via I2C for game status and instructions.
* **Audio Feedback:** Distinct tones for each LED and game events (Win/Loss/Level Up).
* **State Machine Logic:** Robust code structure handling Idle, Level Selection, and Gameplay states.
* **Debouncing:** Software debouncing implemented for reliable button presses.

## 🛠️ Hardware Requirements

* Arduino Uno (or compatible board)
* 1x 16x2 LCD Display with I2C Interface
* 4x Push Buttons (Game Inputs)
* 1x Push Button (Start/Reset)
* 4x LEDs (Red, Green, Blue, Yellow)
* 4x Resistors (220Ω for LEDs)
* 1x Piezo Buzzer
* Jumper Wires & Breadboard

## 🔌 Circuit & Wiring

Please follow the wiring diagram and pin mapping below to connect all components.

![Arduino Simon Says Circuit Diagram](Circuit_diagram.png)

### Pin Mapping Table

| Component | Pin Type | Arduino Pin | Connection Details |
| :--- | :--- | :--- | :--- |
| **I2C LCD** | VCC | 5V | Power |
| **I2C LCD** | GND | GND | Ground |
| **I2C LCD** | SDA | A4 | Data Line (I2C) |
| **I2C LCD** | SCL | A5 | Clock Line (I2C) |
| **Start Button** | Digital | 12 | Button Leg A (Leg B to GND) |
| **Buzzer** | PWM/Digital| 10 | Positive Leg (Negative to GND) |
| **Button 1** | Digital | 2 | Button Leg A (Leg B to GND) |
| **LED 1** | Digital | 3 | Anode (+) via 220Ω Resistor |
| **Button 2** | Digital | 4 | Button Leg A (Leg B to GND) |
| **LED 2** | Digital | 5 | Anode (+) via 220Ω Resistor |
| **Button 3** | Digital | 6 | Button Leg A (Leg B to GND) |
| **LED 3** | Digital | 7 | Anode (+) via 220Ω Resistor |
| **Button 4** | Digital | 8 | Button Leg A (Leg B to GND) |
| **LED 4** | Digital | 9 | Anode (+) via 220Ω Resistor |

**Note on Buttons:** The code uses `INPUT_PULLUP`, so you do **not** need external pull-up resistors. Simply connect one leg of the button to the defined pin and the other leg to GND.

## 🕹️ How to Play

1.  **Power On:** Connect the Arduino to power. The LCD will prompt "PRESS START".
2.  **Start Game:** Press the Start Button (Pin 12).
3.  **Select Level:** Use the 4 game buttons to highlight a difficulty level. Press the same button again to confirm your selection.
4.  **Memorize:** Watch the LED sequence and listen to the tones.
5.  **Repeat:** Press the buttons in the exact order shown.
6.  **Progress:** If correct, the level advances, and the sequence gets longer. If wrong, the Game Over screen appears with your score.

## 💻 Code Structure

The code is built around a Finite State Machine (FSM) with three main states:
1.  `IDLE`: Waiting for the user to start.
2.  `SELECT_LEVEL`: User chooses the difficulty.
3.  `GAME_RUNNING`: The core game loop handling sequences and input validation.

## 📜 License

This project is open-source. Feel free to use and modify it for your own learning!

---
*Created by [Malhar Salunkhe](https://www.linkedin.com/in/malhar-salunkhe/)*
