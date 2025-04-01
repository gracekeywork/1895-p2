#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);  // RX, TX for DFPlayer
DFPlayerMini_Fast myMP3;

const int switch_pin = 2;
const int jack_pin = 4;
const int tune_pin = A0;
const int strum_pin = A2;
const int LED_task = 6;
const int LED_fail = 7;

int score = 0;
unsigned long timeLimit = 10000;
const int maxScore = 99;
const int strum_threshold = 500;
const int minTimeLimit = 3000;

int switch_state = 0;

void setup() {
  pinMode(switch_pin, INPUT);
  pinMode(jack_pin, INPUT_PULLUP);
  pinMode(tune_pin, INPUT);
  pinMode(strum_pin, INPUT);
  pinMode(LED_task, OUTPUT);
  pinMode(LED_fail, OUTPUT);

  Serial.begin(9600);
  mySerial.begin(9600);  // Initialize SoftwareSerial

  if (!myMP3.begin(mySerial)) {
    Serial.println("DFPlayer not connected or SD missing.");
    while (true);
  }

  myMP3.volume(20);  // Volume: 0–30
  randomSeed(analogRead(A1));  // Random seed
}

void playSound(const char* sound) {
  if (strcmp(sound, "Tune Sound") == 0) {
    myMP3.play(1);
  } else if (strcmp(sound, "Strum Sound") == 0) {
    myMP3.play(2);
  } else if (strcmp(sound, "Plug Sound") == 0) {
    myMP3.play(3);
  } else if (strcmp(sound, "Game Over Sound") == 0) {
    myMP3.play(4);
  } else if (strcmp(sound, "Game Start Sound") == 0) {
    myMP3.play(5);
  } else if (strcmp(sound, "You Won Sound") == 0) {
    myMP3.play(6);
  } else if (strcmp(sound, "You Lost Sound") == 0) {
    myMP3.play(7);
  }
}

void gameOver(bool won = false) {
  digitalWrite(LED_task, LOW);
  digitalWrite(LED_fail, HIGH);

  if (won) {
    Serial.println("YOU WON!");
    playSound("You Won Sound");
  } else {
    Serial.println("Game Over!");
    playSound("You Lost Sound");
  }

  delay(3000);  // Let the sound finish
  while (true);  // Freeze game
}

void loop() {
  int stableSwitchState = digitalRead(switch_pin);
  delay(50);
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
  unsigned long lastActionTime = millis();
  unsigned long timeout = timeLimit;

  switch (action) {
    case 1:  // Tune it
      Serial.println("Tune it!");
      playSound("Tune Sound");
      while (millis() - lastActionTime < timeout) {
        int current_tune_val = analogRead(tune_pin);
        if (current_tune_val > 10) {
          Serial.println("Good job!");
          score++;
          timeout = max(minTimeLimit, timeout - 50);
          if (score >= maxScore) {
            gameOver(true);
          }
          digitalWrite(LED_task, HIGH);
          delay(500);
          digitalWrite(LED_task, LOW);
          return;
        }
      }
      break;

    case 2:  // Strum it
      Serial.println("Strum it!");
      playSound("Strum Sound");
      while (millis() - lastActionTime < timeout) {
        int current_strum_val = analogRead(strum_pin);
        if (current_strum_val > strum_threshold) {
          Serial.println("Good job!");
          score++;
          timeout = max(minTimeLimit, timeout - 50);
          if (score >= maxScore) {
            gameOver(true);
          }
          digitalWrite(LED_task, HIGH);
          delay(500);
          digitalWrite(LED_task, LOW);
          return;
        }
      }
      break;

    case 3:  // Plug it
      Serial.println("Plug it!");
      playSound("Plug Sound");
      while (millis() - lastActionTime < timeout) {
        if (digitalRead(jack_pin) == LOW) {
          Serial.println("Good job!");
          score++;
          timeout = max(minTimeLimit, timeout - 50);
          if (score >= maxScore) {
            gameOver(true);
          }
          digitalWrite(LED_task, HIGH);
          delay(500);
          digitalWrite(LED_task, LOW);
          return;
        }
      }
      break;
  }

  // If we reach here, time ran out and player failed the task
  gameOver(false);
}
