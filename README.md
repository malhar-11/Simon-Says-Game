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

## 🔌 Pin Configuration

| Component | Arduino Pin | Description |
| :--- | :--- | :--- |
| **Start Button** | 12 | Initiates game / Resets |
| **Buzzer** | 10 | Audio output |
| **Button 1** | 2 | Input 1 |
| **LED 1** | 3 | Output 1 |
| **Button 2** | 4 | Input 2 |
| **LED 2** | 5 | Output 2 |
| **Button 3** | 6 | Input 3 |
| **LED 3** | 7 | Output 3 |
| **Button 4** | 8 | Input 4 |
| **LED 4** | 9 | Output 4 |
| **LCD SDA** | A4 | I2C Data |
| **LCD SCL** | A5 | I2C Clock |

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
