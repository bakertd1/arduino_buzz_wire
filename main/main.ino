enum GameState {
  INITIAL,
  FAILED,
  IN_PROGRESS,
  SUCCESS
};

const byte PIN_LED_RED = 11;
const byte PIN_LED_GREEN = 10;
const byte PIN_LED_BLUE = 9;

const byte PIN_BUZZER = 3;

const byte PIN_START_ZONE = 4;
const byte PIN_FAIL_ZONE = 5;
const byte PIN_END_ZONE = 6;

GameState gameState = GameState::INITIAL;

void setup() {
  // establish pins for the RGB LED
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

  // establish pin for the buzzer
  pinMode(PIN_BUZZER, OUTPUT);

  // establish pins for the game zones
  pinMode(PIN_START_ZONE, INPUT_PULLUP);
  pinMode(PIN_FAIL_ZONE, INPUT_PULLUP);
  pinMode(PIN_END_ZONE, INPUT_PULLUP);

  // establish serial connection for debugging purposes
  Serial.begin(9600);

  setInitialGameState();
}

void loop() {
  switch (gameState) {
    case GameState::INITIAL:
      listenForGameReset();
      break;
    case GameState::IN_PROGRESS:
      loopInProgressGameState();
      break;
    case GameState::FAILED:
      listenForGameReset();
      break;
    case GameState::SUCCESS:
      listenForGameReset();
      break; 
  }
}

/**
 * Set everything up for the initial game state
 */
void setInitialGameState() {
  Serial.write("In initial game state\n");
  setLedColorBlue();
  // play tones to signify game start
  tone(PIN_BUZZER, 975, 500);
  delay(500);
  noTone(PIN_BUZZER);
}

/**
 * Call when the game is in a state that requires the loop to
 * be returned to the start zone
 */
void listenForGameReset() {
  // check to see if the loop is touching the start zone
  if (!digitalRead(PIN_START_ZONE)) {
    // the loop is touching the start zone so change the game start
    Serial.write("Game reset\n");
    changeStateToInProgress();
  }
}

/**
 * Call this when in the in progress game state loop
 */
void loopInProgressGameState() {
  // listen for a game state change
  if (!digitalRead(PIN_END_ZONE)) {
    // the loop is touching the end zone so the player was successful
    Serial.write("Player won\n");
    changeStateToSuccess();
  } else if (!digitalRead(PIN_FAIL_ZONE)) {
    // the loop touched the pipe so the player was unsuccessful
    Serial.write("Player lost\n");
    changeStateToFailed();
  }
}

/**
 * Call this when the loop is in the start zone while a game isn't already in progress
 */
void changeStateToInProgress() {
  gameState = GameState::IN_PROGRESS;
  Serial.write("State set to in progress\n");
  setLedColorYellow();

  // play tones to signify game start
  tone(PIN_BUZZER, 975, 200);
  delay(200);
  tone(PIN_BUZZER, 1210, 800);
  delay(800);
  noTone(PIN_BUZZER);

  delay(500);
  setLedColorOff();
}

/**
 * Call this when the loop has touched the pipe and the game state is in progress
 */
void changeStateToFailed() {
  gameState = GameState::FAILED;
  Serial.write("State set to failed\n");
  setLedColorRed();

  tone(PIN_BUZZER, 1000, 500);
  delay(500);
  tone(PIN_BUZZER, 925, 500);
  delay(500);
  tone(PIN_BUZZER, 850, 800);
  delay(800);
  noTone(PIN_BUZZER);
}

/**
 * Call this when the loop has touched the pipe and the game state is in progress
 */
void changeStateToSuccess() {
  gameState = GameState::SUCCESS;
  Serial.write("State set to success\n");
  setLedColorGreen();

  tone(PIN_BUZZER, 1200, 200);
  delay(250);
  tone(PIN_BUZZER, 1200, 200);
  delay(250);
  tone(PIN_BUZZER, 1200, 200);
  delay(250);
  tone(PIN_BUZZER, 1600, 500);
  delay(550);
  tone(PIN_BUZZER, 1200, 200);
  delay(250);
  tone(PIN_BUZZER, 1600, 800);
  delay(800);
  noTone(PIN_BUZZER);
}

void setLedColorYellow() {
  setLedColor(255, 25, 0);
}

void setLedColorRed() {
  setLedColor(50, 0, 0);
}

void setLedColorGreen() {
  setLedColor(0, 5, 0);
}

void setLedColorBlue() {
  setLedColor(0, 0, 50);
}

void setLedColorOff() {
  setLedColor(0, 0, 0);
}

void setLedColor(int red, int green, int blue) {
  analogWrite(PIN_LED_RED, red);
  analogWrite(PIN_LED_GREEN, green);
  analogWrite(PIN_LED_BLUE, blue);
}
