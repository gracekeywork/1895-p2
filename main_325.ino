const int switch_pin = 2;  // On/Off switch (game control)
const int jack_pin = 4;    // Plug It jack detection
const int tune_pin = A0;   // Tune It potentiometer
const int strum_pin = A2;  // Strum It microphone 
const int LED_task = 6;    // LED to indicate task success
const int LED_fail = 7;    // LED for failure indication

int score = 0;  // Score initialized to 0
unsigned long startTime = 0;  // Timer start
unsigned long timeLimit = 10000;  // Start with 10 seconds
const int maxScore = 99;          // Winning score
const int strum_threshold = 500;  // Adjust this based on testing

int tune_val = 0, prev_tune_val = 0;
int switch_state = 0, jack_state = 0, strum_val = 0;

const int minTimeLimit = 3000;   // 3 seconds minimum time limit (in milliseconds)

void setup() {
  pinMode(switch_pin, INPUT);         // On/Off switch input
  pinMode(jack_pin, INPUT_PULLUP);    // Jack input 
  pinMode(strum_pin, INPUT);          // Strum microphone input
  pinMode(tune_pin, INPUT);           // Potentiometer input 

  pinMode(LED_task, OUTPUT);
  pinMode(LED_fail, OUTPUT);

  Serial.begin(9600);

  randomSeed(analogRead(A1));  // Seed randomness using noise
}

void playSound(const char* sound) {
  Serial.println(sound);  // Placeholder for sound effect
}

void gameOver() {
  Serial.println("Game Over!");
  playSound("Game Over Sound");
  while (true);
}

void loop() {
  int stableSwitchState = digitalRead(switch_pin);
  delay(50);  // Small debounce delay

  if (stableSwitchState == digitalRead(switch_pin)) {
    switch_state = stableSwitchState;
  }

  if (switch_state == LOW) {
    digitalWrite(LED_fail, LOW);
    digitalWrite(LED_task, HIGH);
    Serial.println("Switch is off. Game is paused.");
    return;
  }

  int action = random(1, 4);
  jack_state = digitalRead(jack_pin);
  tune_val = analogRead(tune_pin);
  strum_val = analogRead(strum_pin);

  unsigned long lastActionTime = millis();
  unsigned long timeout = timeLimit;

  switch (action) {
    case 1:
      Serial.println("Tune it!");
      playSound("Tune Sound");
      while (millis() - lastActionTime < timeout) {
        int current_tune_val = analogRead(tune_pin);
        if (current_tune_val > 10) {
          Serial.println("Good job!");
          score++;
          timeout = max(minTimeLimit, timeout - 50);
          if (score >= maxScore) {
            Serial.println("YOU WON!");
            gameOver();
          }
          digitalWrite(LED_task, HIGH);
          delay(500);
          digitalWrite(LED_task, LOW);
          return;
        }
      }
      digitalWrite(LED_fail, HIGH);
      delay(500);
      digitalWrite(LED_fail, LOW);
      break;
    case 2:
      Serial.println("Strum it!");
      playSound("Strum Sound");
      while (millis() - lastActionTime < timeout) {
        int current_strum_val = analogRead(strum_pin);
        if (current_strum_val > strum_threshold) {
          Serial.println("Good job!");
          score++;
          timeout = max(minTimeLimit, timeout - 50);
          if (score >= maxScore) {
            Serial.println("YOU WON!");
            gameOver();
          }
          digitalWrite(LED_task, HIGH);
          delay(500);
          digitalWrite(LED_task, LOW);
          return;
        }
      }
      digitalWrite(LED_fail, HIGH);
      delay(500);
      digitalWrite(LED_fail, LOW);
      break;
    case 3:
      Serial.println("Plug it!");
      playSound("Plug Sound");
      while (millis() - lastActionTime < timeout) {
        if (digitalRead(jack_pin) == LOW) {
          Serial.println("Good job!");
          score++;
          timeout = max(minTimeLimit, timeout - 50);
          if (score >= maxScore) {
            Serial.println("YOU WON!");
            gameOver();
          }
          digitalWrite(LED_task, HIGH);
          delay(500);
          digitalWrite(LED_task, LOW);
          return;
        }
      }
      digitalWrite(LED_fail, HIGH);
      delay(500);
      digitalWrite(LED_fail, LOW);
      break;
  }

  gameOver();
}
