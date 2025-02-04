// Multi-Game Device Code for ESP32
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button Pins
#define BUTTON_UP 23
#define BUTTON_DOWN 4
#define BUTTON_SELECT 0

// Game States
enum Screen {SPLASH, MENU, RPS, GUESS, CATCHER};
Screen currentScreen = SPLASH;

// Menu Navigation
int menuIndex = 0;
const char* games[] = {"Rock-Paper-Scissors", "Guess the Number", "The Catcher"};
const int totalGames = 3;

// Button State Tracking
bool lastButtonUp = HIGH, lastButtonDown = HIGH, lastButtonSelect = HIGH;

// Guess the Number Variables
int guess = 0;
int targetNumber = 0;
bool guessing = true;

// The Catcher Variables
int catcherX = SCREEN_WIDTH / 2;
int fallingObjectX, fallingObjectY;
int score = 0;

// Setup Function
void setup() {
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  displaySplashScreen();
  currentScreen = MENU;
  displayMenu();
}

// Splash Screen
void displaySplashScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 25);
  display.print("Multi-Game Hub");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

// Menu Screen
void displayMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  for (int i = 0; i < totalGames; i++) {
    display.setCursor(10, i * 12 + 10);
    if (i == menuIndex) display.print("> ");
    else display.print("  ");
    display.println(games[i]);
  }
  display.display();
}

// Handle Button Inputs
void handleMenuNavigation() {
  bool currentButtonUp = digitalRead(BUTTON_UP);
  bool currentButtonDown = digitalRead(BUTTON_DOWN);
  bool currentButtonSelect = digitalRead(BUTTON_SELECT);

  if (lastButtonUp == HIGH && currentButtonUp == LOW) {
    menuIndex = (menuIndex - 1 + totalGames) % totalGames;
    displayMenu();
    delay(200);
  }
  if (lastButtonDown == HIGH && currentButtonDown == LOW) {
    menuIndex = (menuIndex + 1) % totalGames;
    displayMenu();
    delay(200);
  }
  if (lastButtonSelect == HIGH && currentButtonSelect == LOW) {
    currentScreen = (Screen)(menuIndex + 2);
    delay(200);
  }

  lastButtonUp = currentButtonUp;
  lastButtonDown = currentButtonDown;
  lastButtonSelect = currentButtonSelect;
}

// Wait for Select Button to Return to Menu
void waitForSelectButton() {
  display.setCursor(0, 50);
  display.println("Press Select to return");
  display.display();

  while (digitalRead(BUTTON_SELECT) == HIGH);
  delay(200);  // Debounce delay
}

// Rock-Paper-Scissors Game
void playRPSGame() {
  int choice = 0;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Choose:");
  display.println("1. Rock");
  display.println("2. Paper");
  display.println("3. Scissors");
  display.display();

  while (true) {
    bool currentButtonUp = digitalRead(BUTTON_UP);
    bool currentButtonDown = digitalRead(BUTTON_DOWN);
    bool currentButtonSelect = digitalRead(BUTTON_SELECT);

    if (lastButtonUp == HIGH && currentButtonUp == LOW) {
      choice = (choice + 2) % 3;
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Choose:");
      display.println("1. Rock");
      display.println("2. Paper");
      display.println("3. Scissors");
      display.setCursor(0, 40);
      display.print("Current: ");
      if (choice == 0) display.println("Rock");
      else if (choice == 1) display.println("Paper");
      else display.println("Scissors");
      display.display();
      delay(200);
    }
    if (lastButtonDown == HIGH && currentButtonDown == LOW) {
      choice = (choice + 1) % 3;
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Choose:");
      display.println("1. Rock");
      display.println("2. Paper");
      display.println("3. Scissors");
      display.setCursor(0, 40);
      display.print("Current: ");
      if (choice == 0) display.println("Rock");
      else if (choice == 1) display.println("Paper");
      else display.println("Scissors");
      display.display();
      delay(200);
    }
    if (lastButtonSelect == HIGH && currentButtonSelect == LOW) {
      int computerChoice = random(0, 3);  // Random choice for computer
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("You chose: ");
      if (choice == 0) display.println("Rock");
      else if (choice == 1) display.println("Paper");
      else display.println("Scissors");

      display.print("Computer chose: ");
      if (computerChoice == 0) display.println("Rock");
      else if (computerChoice == 1) display.println("Paper");
      else display.println("Scissors");

      // Determine the winner
      if (choice == computerChoice) {
        display.println("It's a tie!");
      } else if ((choice == 0 && computerChoice == 2) ||
                 (choice == 1 && computerChoice == 0) ||
                 (choice == 2 && computerChoice == 1)) {
        display.println("You win!");
      } else {
        display.println("You lose!");
      }
      display.display();
      delay(3000);
      break;
    }

    lastButtonUp = currentButtonUp;
    lastButtonDown = currentButtonDown;
    lastButtonSelect = currentButtonSelect;
  }
  waitForSelectButton();
}

// Guess the Number Game
void playGuessGame() {
  targetNumber = random(0, 10);
  guess = 0;
  guessing = true;

  while (guessing) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Guess: ");
    display.println(guess);
    display.display();

    bool currentButtonUp = digitalRead(BUTTON_UP);
    bool currentButtonDown = digitalRead(BUTTON_DOWN);
    bool currentButtonSelect = digitalRead(BUTTON_SELECT);

    if (lastButtonUp == HIGH && currentButtonUp == LOW) {
      guess = (guess + 1) % 10;
      delay(200);
    }
    if (lastButtonDown == HIGH && currentButtonDown == LOW) {
      guess = (guess - 1 + 10) % 10;
      delay(200);
    }
    if (lastButtonSelect == HIGH && currentButtonSelect == LOW) {
      display.clearDisplay();
      display.setCursor(0, 0);
      if (guess == targetNumber) {
        display.println("Correct!");
      } else {
        if (guess > targetNumber) {
          display.println("Too High!");
        } else {
          display.println("Too Low!");
        }
      }
      display.display();
      delay(2000);
      if (guess == targetNumber) guessing = false;
    }

    lastButtonUp = currentButtonUp;
    lastButtonDown = currentButtonDown;
    lastButtonSelect = currentButtonSelect;
  }
  waitForSelectButton();
}

// The Catcher Game
void playCatcherGame() {
  fallingObjectX = random(0, SCREEN_WIDTH);
  fallingObjectY = 0;
  catcherX = SCREEN_WIDTH / 2;
  score = 0;

  while (true) {
    display.clearDisplay();
    display.fillRect(catcherX, SCREEN_HEIGHT - 5, 10, 3, SSD1306_WHITE);
    display.fillCircle(fallingObjectX, fallingObjectY, 2, SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Score: ");
    display.println(score);
    display.display();

    bool currentButtonUp = digitalRead(BUTTON_UP);
    bool currentButtonDown = digitalRead(BUTTON_DOWN);

    if (lastButtonUp == HIGH && currentButtonUp == LOW) {
      catcherX = max(0, catcherX - 5);
    }
    if (lastButtonDown == HIGH && currentButtonDown == LOW) {
      catcherX = min(SCREEN_WIDTH - 10, catcherX + 5);
    }

    fallingObjectY += 2;
    if (fallingObjectY >= SCREEN_HEIGHT - 5) {
      if (fallingObjectX >= catcherX && fallingObjectX <= catcherX + 10) {
        score++;
      }
      fallingObjectX = random(0, SCREEN_WIDTH);
      fallingObjectY = 0;
    }

    lastButtonUp = currentButtonUp;
    lastButtonDown = currentButtonDown;

    if (digitalRead(BUTTON_SELECT) == LOW) break;
    delay(100);
  }
  waitForSelectButton();
}

// Main Loop
void loop() {
  if (currentScreen == MENU) {
    handleMenuNavigation();
  } else if (currentScreen == RPS) {
    playRPSGame();
    currentScreen = MENU;
    displayMenu();
  } else if (currentScreen == GUESS) {
    playGuessGame();
    currentScreen = MENU;
    displayMenu();
  } else if (currentScreen == CATCHER) {
    playCatcherGame();
    currentScreen = MENU;
    displayMenu();
  }
}