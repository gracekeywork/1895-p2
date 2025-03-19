const int switch_pin = 2;  // On/Off switch (game control)
const int jack_pin = 4;    // Plug It jack detection
const int tune_pin = A0;   // Tune It potentiometer
const int strum_pin = A2;  // Strum It microphone 
const int LED_task = 6;    // LED to indicate task success
const int LED_fail = 7;    // LED for failure indication

const int strum_threshold = 500;  // Adjust this based on testing
const int maxScore = 99;          // Winning score

int tune_val = 0, prev_tune_val = 0;
int switch_state = 0, jack_state = 0, strum_val = 0;
int currentTask = -1;  // Store the selected task
unsigned long startTime = 0;  // Timer start
unsigned long timeLimit = 10000;  // Start with 10 seconds
const int minTimeLimit = 3000;   // 3 seconds minimum time limit (in milliseconds)
int score = 0;  // Score initialized to 0

void setup() {
  pinMode(switch_pin, INPUT);         // On/Off switch input
  pinMode(jack_pin, INPUT_PULLUP);    // Jack input 
  pinMode(strum_pin, INPUT);          // Strum microphone input
  pinMode(tune_pin, INPUT);           // Potentiometer input 

  pinMode(LED_task, OUTPUT);
  pinMode(LED_fail, OUTPUT);

  Serial.begin(9600);

  randomSeed(analogRead(A1));  // Seed randomness using noise
  selectNewTask();  // Pick the first task
}

void loop() {
  // Debounce the on/off switch
  int stableSwitchState = digitalRead(switch_pin);
  delay(50);  // Small debounce delay
  if (stableSwitchState == digitalRead(switch_pin)) {
    switch_state = stableSwitchState;
  }

  if (switch_state == LOW) {
    // If switch is off, pause the game
    digitalWrite(LED_fail, HIGH);  // Indicate game is paused (failure LED on)
    Serial.println("Switch is off. Game is paused.");
    return;  // Exit the loop and do nothing until the switch is on
  }

  // If the switch is on, game is active
  digitalWrite(LED_fail, LOW);  // Turn off failure LED (game is active)

  // Read inputs for the active task
  jack_state = digitalRead(jack_pin);
  tune_val = analogRead(tune_pin);
  strum_val = analogRead(strum_pin);

  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;  // Time passed since task started

  // Check if the player completed the task
  bool taskCompleted = false;

  if (currentTask == 0 && tune_val != prev_tune_val) {  // Tune It
    Serial.println("Tune It Completed!");
    taskCompleted = true;
  }
  else if (currentTask == 1 && strum_val > strum_threshold) {  // Strum It
    Serial.println("Strum It Completed!");
    taskCompleted = true;
  }
  else if (currentTask == 2 && jack_state == LOW) {  // Plug It
    Serial.println("Plug It Completed!");
    taskCompleted = true;
  }

  // Handle task completion
  if (taskCompleted) {
    digitalWrite(LED_task, HIGH);
    delay(500);  // Short success indicator
    digitalWrite(LED_task, LOW);

    // Increase score
    if (score < maxScore) {
      score++;
    }

    // Check for winning condition
    if (score == maxScore) {
      Serial.println("YOU WON!");
      delay(2000);  // Short delay before restart
      resetGame();
      return;
    }

    // Decrease the time for the next task, but not below 3 seconds
    if (timeLimit > minTimeLimit) {
      timeLimit = max(minTimeLimit, (int)(timeLimit * 0.9));  // Reduce by 10%
    }

    selectNewTask();  // Pick a new task
  }

  // Display the remaining time (in seconds) and score on Serial Monitor
  Serial.print("Time Left: ");
  Serial.print((timeLimit - elapsedTime) / 1000.0, 3);  // Display remaining time in seconds
  Serial.print(" Score: ");
  Serial.println(score);

  // Check for timeout (if the time has elapsed)
  if (elapsedTime >= timeLimit) {
    Serial.println("Time's up! Task Failed.");
    
    // Blink the failure LED 3 times
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_fail, HIGH);
      delay(200);
      digitalWrite(LED_fail, LOW);
      delay(200);
    }

    // Reset score and time if task was not completed in time
    score = 0;
    timeLimit = 10000;  // Reset time to 10 seconds

    selectNewTask();  // Pick a new task
  }

  prev_tune_val = tune_val;  // Update previous tune value
}

void selectNewTask() {
  currentTask = random(3);  // Choose a random task (0-2)
  startTime = millis();     // Restart timer

  // Print selected task
  if (currentTask == 0) Serial.println("New Task: Tune It!");
  else if (currentTask == 1) Serial.println("New Task: Strum It!");
  else if (currentTask == 2) Serial.println("New Task: Plug It!");
}

void resetGame() {
  score = 0;
  timeLimit = 10000;  // Reset time to 10 seconds
  Serial.println("Game Restarted!");
  selectNewTask();
}
