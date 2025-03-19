void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


//FUNCTION PROTOTYPES:
//=================================================================================================================================================================================
int DebounceButton(int buttonPin, int &buttonState, int &lastButtonState, unsigned long &lastDebounceTime, unsigned long debounceDelay){
  // Read the state of the button
  int reading = digitalRead(buttonPin);
  int isPressed = 0;
  
  // Check if the button state has changed (i.e., from HIGH to LOW or LOW to HIGH)
  if (reading != lastButtonState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }

  // Check if the state has been stable for long enough (debounce time)
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the state has changed and is stable, update the button state
    if (reading != buttonState) {
      buttonState = reading;

      // Only act on the change if the new state is HIGH (button pressed)
      if (buttonState == HIGH) {
        isPressed = 1;
      }
    }
  }
  
  // Save the reading for the next loop iteration
  lastButtonState = reading;
  return isPressed;
}

void ToggleDebounceButton(bool &ledState, int ledPin, int buttonPin, int &buttonState, int &lastButtonState, unsigned long &lastDebounceTime, unsigned long debounceDelay){
  // Read the state of the button
  int reading = digitalRead(buttonPin);
  
  // Check if the button state has changed (i.e., from HIGH to LOW or LOW to HIGH)
  if (reading != lastButtonState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }

  // Check if the state has been stable for long enough (debounce time)
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the state has changed and is stable, update the button state
    if (reading != buttonState) {
      buttonState = reading;

      // Only act on the change if the new state is HIGH (button pressed)
      if (buttonState == HIGH) {
        //isPressed = 1;
        ledState = !ledState;  // Toggle the LED state
        digitalWrite(ledPin, ledState);  // Set the LED based on the new state
      }
    }
  }

  float GetConvertedPotValue(int actual_pot_value, int actual_min, float actual_max, int desired_range_min, float desired_range_max) {
    // Clamp the potentiometer value within the actual min/max range
    if (float(actual_pot_value) > actual_max) {
        actual_pot_value = actual_max;
    } else if (actual_pot_value < actual_min) {
        actual_pot_value = actual_min;
    }

    // Calculate the actual and desired ranges
    float actual_range = actual_max - actual_min;
    float desired_range = desired_range_max - desired_range_min;

    // Map the actual potentiometer value to the 0-1 range within the actual range
    float normalized_value = float(actual_pot_value - actual_min) / float(actual_range);

    // Convert to the desired range
    float converted_value = (normalized_value * desired_range) + desired_range_min;

    return converted_value;
}
