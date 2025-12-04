/*
 * Simon Says Game - Arduino Project
 * Author: Malhar Salunkhe
 * * Features: 
 * - 4 Difficulty Levels
 * - I2C LCD Interface
 * - Audio/Visual Feedback
 * - State Machine Logic
 */

#include <LiquidCrystal_I2C.h>

// Initialize LCD address 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Pin Definitions ---
// Using arrays makes it easier to iterate through LEDs and Buttons in loops
int LED[4] = {3, 5, 7, 9};       
int Button[4] = {2, 4, 6, 8};    
int ButtonStates[4];             // Stores the current reading of buttons
int StartPin = 12;               // The separate Start/Reset button
int buzz = 10;                   // Piezo Buzzer pin

// --- Game Variables ---
int StartVal;
bool GameState = false;          // General flag to check if game is active
int Score = 0;
int Level;
int Steps;
int Notes;

// --- Pre-defined Game Sequences ---
// These arrays hold the pattern the player must mimic for each difficulty
int easy[5]    = {0, 1, 2, 1, 3};
int medium[10] = {0, 1, 2, 3, 1, 2, 0, 3, 2, 1};
int hard[15]   = {0, 1, 0, 2, 3, 1, 2, 3, 0, 1, 2, 0, 3, 1, 0};
int insane[20] = {0, 1, 2, 3, 0, 2, 1, 3, 2, 0, 1, 3, 0, 2, 3, 1, 2, 0, 3, 1};

// Tones corresponding to the 4 buttons (C4, E4, G4, B4)
int Buzz[4] = {261, 330, 392, 494};

// Level Selection Variables
int Selected = -1;
bool Confirmed = false;

// --- State Machine ---
// Using an Enum allows us to easily track distinct phases of the program
enum GamePhase {
  IDLE,
  SELECT_LEVEL,
  GAME_RUNNING
};

GamePhase currentPhase = IDLE;

// --- Function Prototypes ---
void lcdReset();
void lvlPrinter();
void LevelSelector();
void Game(int n);

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("PRESS START");

  // Initialize Inputs and Outputs
  for (int i = 0; i < 4; i++) {
    pinMode(Button[i], INPUT_PULLUP); // Use internal pullup resistors
    pinMode(LED[i], OUTPUT);
  }
  pinMode(StartPin, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
}

void loop() {
  StartVal = digitalRead(StartPin);

  // --- Start / Reset Button Logic ---
  if (StartVal == LOW) { // Button pressed (Active LOW due to INPUT_PULLUP)
    delay(200); // Simple debounce
    
    if (!GameState) {
      // START THE GAME
      tone(buzz, 523, 300); // Start sound (C5)
      lcdReset();
      lcd.print("GAME STARTED");
      delay(2000);
      lcdReset();
      lcd.print("SELECT LEVEL");
      
      // Initialize Game State
      GameState = true;
      currentPhase = SELECT_LEVEL;
      Level = 0;
      Selected = -1;
      Confirmed = false;
      
    } else {
      // RESET THE GAME (if pressed while playing)
      tone(buzz, 784, 100); delay(120);
      tone(buzz, 523, 100); delay(120);
      lcdReset();
      lcd.print("GAME RESET");
      
      delay(1000);
      lcdReset();
      lcd.print("PRESS START");
      
      // Reset State
      GameState = false;
      currentPhase = IDLE;
      Score = 0;
    }
    
    // Wait until button is released so we don't re-trigger immediately
    while (digitalRead(StartPin) == LOW);
    delay(200);
  }

  // If game isn't running, don't execute the rest of the loop
  if (!GameState) return;

  // --- Main State Machine Handler ---
  if (currentPhase == SELECT_LEVEL) {
    LevelSelector();
  }
  else if (currentPhase == GAME_RUNNING) {
    lvlPrinter();
    Game(Level);
  }
}

// Helper to clear LCD and reset cursor
void lcdReset() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

// Helper to display level names during selection
void LevelLCD(int n) {
  lcdReset();
  switch (n) {
    case 1: lcd.print(">EASY"); break;
    case 2: lcd.print(">MEDIUM"); break;
    case 3: lcd.print(">HARD"); break;
    case 4: lcd.print(">INSANE"); break;
  }
}

// Logic for choosing difficulty using the 4 game buttons
void LevelSelector() {
  for (int i = 0; i < 4; i++) {
    ButtonStates[i] = digitalRead(Button[i]);
    
    if (ButtonStates[i] == LOW) {
      delay(200); // Debounce
      
      if (Selected == -1) {
        // First press: Highlight the level
        Selected = i;
        Level = i + 1;
        LevelLCD(Level);
        lcd.setCursor(0, 1);
        lcd.print("Press again...");
        
      } else if (Selected == i) {
        // Second press (same button): Confirm selection
        lcdReset();
        lcd.print("LEVEL ");
        lcd.print(Level);
        lcd.setCursor(0, 1);
        lcd.print("CONFIRMED!");
        
        // Confirmation sound sequence
        tone(buzz, 659, 200); delay(120);
        tone(buzz, 784, 200); delay(120);
        tone(buzz, 880, 200); delay(120);
        
        delay(1000);
        lcdReset();
        lcd.print("GAME ON!");
        delay(1000);
        lcdReset();
        currentPhase = GAME_RUNNING;
        break;
        
      } else {
        // Different button pressed: Switch selection
        Selected = i;
        Level = i + 1;
        LevelLCD(Level);
        lcd.setCursor(0, 1);
        lcd.print("Press again...");
      }
      
      while (digitalRead(Button[i]) == LOW); // Wait for release
      delay(200);
      break;
    }
  }
}

// Updates the Score and Level on the LCD during gameplay
void lvlPrinter() {
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(Level);
  lcd.print("     "); // Clear trailing characters
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(Score);
  lcd.print("     ");
}

// --- Main Game Logic ---
void Game(int n) {
  int Notes = 0;
  int* sequence; // Pointer to the selected difficulty array

  // Map difficulty level to specific array and length
  switch (n) {
    case 1: Notes = 5; sequence = easy; break;
    case 2: Notes = 10; sequence = medium; break;
    case 3: Notes = 15; sequence = hard; break;
    case 4: Notes = 20; sequence = insane; break;
  }

  // Iterate through the sequence length (Notes)
  for (int i = 0; i < Notes; i++) {
    
    // --- Phase 1: Show the Sequence ---
    for (int j = 0; j <= i; j++) {
      Steps = sequence[j];
      digitalWrite(LED[Steps], HIGH);
      tone(buzz, Buzz[Steps]);
      delay(500);
      digitalWrite(LED[Steps], LOW);
      noTone(buzz);
      delay(300);
    }

    // --- Phase 2: Wait for Player Input ---
    for (int j = 0; j <= i; j++) {
      bool inputReceived = false;
      int userInput = -1;

      while (!inputReceived) {
        
        // Check for Reset Button during gameplay
        if (digitalRead(StartPin) == LOW) {
          GameState = false;
          currentPhase = IDLE;
          Score = 0;
          lcdReset();
          lcd.print("GAME RESET");
          tone(buzz, 784, 100); delay(120);
          tone(buzz, 523, 100); delay(120);
          delay(1000);
          lcdReset();
          lcd.print("PRESS START");
          return; // Exit function immediately
        }

        // Check Game Buttons
        for (int x = 0; x < 4; x++) {
          if (digitalRead(Button[x]) == LOW) {
            userInput = x;

            // Feedback: Light up LED and play tone
            digitalWrite(LED[x], HIGH);
            tone(buzz, Buzz[x]);
            delay(300);
            digitalWrite(LED[x], LOW);
            noTone(buzz);

            // Debounce: wait for button release
            while (digitalRead(Button[x]) == LOW);
            delay(200);

            inputReceived = true;

            // Validate the move
            if (userInput != sequence[j]) {
              // ❌ WRONG MOVE
              lcdReset();
              lcd.print("WRONG!");
              // Failure sound
              tone(buzz, 880, 100); delay(120);
              tone(buzz, 659, 100); delay(120);
              tone(buzz, 440, 300); delay(320);

              delay(1000);
              lcdReset();
              lcd.print("SCORE: ");
              lcd.print(Score);
              delay(2000);
              lcdReset();
              lcd.print("PRESS START");
              
              // Reset Game State
              GameState = false;
              currentPhase = IDLE;
              Score = 0;
              return;
            }

            lvlPrinter(); // Refresh LCD if move was correct
          }
        }
      }
    }

    // If loop completes, player survived this round
    Score++;
    lvlPrinter();
  }

  // --- Phase 3: Level Complete ---
  lcdReset();
  lcd.print("LEVEL CLEARED!");
  
  // Victory Fanfare
  tone(buzz, 784, 120); delay(140);   
  tone(buzz, 880, 120); delay(140);   
  tone(buzz, 988, 120); delay(140);   
  tone(buzz, 784, 120); delay(140);   
  tone(buzz, 1175, 200); delay(250);  

  delay(1000);
  lcdReset();

  // If not yet at max level (4), proceed to next level
  if (n != 4) {
    Level++;
    lcd.print("NEXT LEVEL");
    delay(1000);
    lcdReset();
    lvlPrinter();
    Game(n + 1); // Recursive call to next level
  } else {
    // Total Victory
    lcd.print("YOU WIN!");
    delay(2000);
    lcdReset();
    lcd.print("PRESS START");
    GameState = false;
    currentPhase = IDLE;
  }
}
