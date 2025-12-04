#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int LED[4] = {3, 5, 7, 9};
int Button[4] = {2, 4, 6, 8};
int ButtonStates[4];
int StartPin = 12;
int StartVal;
bool GameState = false;
int buzz = 10;

int Score = 0;
int Level;
int Steps;
int Notes;

int easy[5]   = {0, 1, 2, 1, 3};
int medium[10] = {0, 1, 2, 3, 1, 2, 0, 3, 2, 1};
int hard[15]  = {0, 1, 0, 2, 3, 1, 2, 3, 0, 1, 2, 0, 3, 1, 0};
int insane[20] = {0, 1, 2, 3, 0, 2, 1, 3, 2, 0, 1, 3, 0, 2, 3, 1, 2, 0, 3, 1};

int Buzz[4] = {261, 330, 392, 494};
int Selected = -1;
bool Confirmed = false;

enum GamePhase {
  IDLE,
  SELECT_LEVEL,
  GAME_RUNNING
};

GamePhase currentPhase = IDLE;

void lcdReset();
void lvlPrinter();
void LevelSelector();
void Game(int n);

void setup() {

  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("PRESS START");

  for (int i = 0; i < 4; i++) {
    pinMode(Button[i], INPUT_PULLUP);
    pinMode(LED[i], OUTPUT);
  }
  pinMode(StartPin, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
}

void loop() {
  StartVal = digitalRead(StartPin);

  if (StartVal == LOW) {
    delay(200);
    if (!GameState) {
      tone(buzz, 523, 300);
      lcdReset();
      lcd.print("GAME STARTED");
      delay(2000);
      lcdReset();
      lcd.print("SELECT LEVEL");
      GameState = true;
      currentPhase = SELECT_LEVEL;
      Level = 0;
      Selected = -1;
      Confirmed = false;
    } else {
      tone(buzz, 784, 100); delay(120);
      tone(buzz, 523, 100); delay(120);
      lcdReset();
      lcd.print("GAME RESET");
      
      delay(1000);
      lcdReset();
      lcd.print("PRESS START");
      GameState = false;
      currentPhase = IDLE;
      Score = 0;
    }
    while (digitalRead(StartPin) == LOW);
    delay(200);
  }

  if (!GameState) return;

  if (currentPhase == SELECT_LEVEL) {
    LevelSelector();
  }
  else if (currentPhase == GAME_RUNNING) {
    lvlPrinter();
    

    Game(Level);
  }
}

void lcdReset() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

void LevelLCD(int n) {
  lcdReset();
  switch (n) {
    case 1: lcd.print(">EASY"); break;
    case 2: lcd.print(">MEDIUM"); break;
    case 3: lcd.print(">HARD"); break;
    case 4: lcd.print(">INSANE"); break;
  }
}

void LevelSelector() {
  for (int i = 0; i < 4; i++) {
    ButtonStates[i] = digitalRead(Button[i]);
    if (ButtonStates[i] == LOW) {
      delay(200);
      if (Selected == -1) {
        Selected = i;
        Level = i + 1;
        LevelLCD(Level);
        lcd.setCursor(0, 1);
        lcd.print("Press again...");
      } else if (Selected == i) {
        lcdReset();
        lcd.print("LEVEL ");
        lcd.print(Level);
        lcd.setCursor(0, 1);
        lcd.print("CONFIRMED!");
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
        Selected = i;
        Level = i + 1;
        LevelLCD(Level);
        lcd.setCursor(0, 1);
        lcd.print("Press again...");
      }
      while (digitalRead(Button[i]) == LOW);
      delay(200);
      break;
    }
  }
}

void lvlPrinter() {
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(Level);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(Score);
  lcd.print("     ");
}

void Game(int n) {
  int Notes = 0;
  int* sequence;

  switch (n) {
    case 1: Notes = 5; sequence = easy; break;
    case 2: Notes = 10; sequence = medium; break;
    case 3: Notes = 15; sequence = hard; break;
    case 4: Notes = 20; sequence = insane; break;
  }

  for (int i = 0; i < Notes; i++) {
    // Flash the sequence from 0 to i
    for (int j = 0; j <= i; j++) {
      Steps = sequence[j];
      digitalWrite(LED[Steps], HIGH);
      tone(buzz, Buzz[Steps]);
      delay(500);
      digitalWrite(LED[Steps], LOW);
      noTone(buzz);
      delay(300);
    }

    // Wait for player's full sequence input
    for (int j = 0; j <= i; j++) {
      bool inputReceived = false;
      int userInput = -1;

      while (!inputReceived) {
        if (digitalRead(StartPin) == LOW) {
          // Reset mid-game
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
          return;
        }

        for (int x = 0; x < 4; x++) {
          if (digitalRead(Button[x]) == LOW) {
            userInput = x;

            digitalWrite(LED[x], HIGH);
            tone(buzz, Buzz[x]);
            delay(300);
            digitalWrite(LED[x], LOW);
            noTone(buzz);

            // 👇 Debounce: wait for release
            while (digitalRead(Button[x]) == LOW);
            delay(200);

            inputReceived = true;

            
            if (userInput != sequence[j]) {
              // ❌ Wrong move
              lcdReset();
              lcd.print("WRONG!");
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
              GameState = false;
              currentPhase = IDLE;
              Score = 0;
              return;
            }

            lvlPrinter();  // ✅ Good move
          }
        }
      }
    }

    Score++;
    lvlPrinter();
  }

  // 🎉 Level complete
  lcdReset();
  lcd.print("LEVEL CLEARED!");
  tone(buzz, 784, 120); delay(140);   // G5
  tone(buzz, 880, 120); delay(140);   // A5
  tone(buzz, 988, 120); delay(140);   // B5
  tone(buzz, 784, 120); delay(140);   // G5 (back down)
  tone(buzz, 1175, 200); delay(250);  // D6 - the power note!

  delay(1000);
  lcdReset();

  if (n != 4) {
    Level++;
    lcd.print("NEXT LEVEL");
    delay(1000);
    lcdReset();
    lvlPrinter();
    Game(n + 1);
  } else {
    lcd.print("YOU WIN!");
    delay(2000);
    lcdReset();
    lcd.print("PRESS START");
    GameState = false;
    currentPhase = IDLE;
  }
}

