const int LED_jack = 12; 
const int jack_pin = 4; 
int jack_state;  

const int LED_on = 8; 
const int switch_pin = 2; 
int switch_state; 

void setup() 
{
  pinMode(LED_on, OUTPUT);
  pinMode(switch_pin, INPUT); 

  pinMode(LED_jack, OUTPUT);
  pinMode(jack_pin, INPUT_PULLUP);

}

void loop() 
{

  jack_state =  digitalRead(jack_pin);


  if(jack_state == HIGH)
  {
    digitalWrite(LED_jack, LOW);
  }
  else
  {
    digitalWrite(LED_jack, HIGH);
  }

   switch_state =  digitalRead(switch_pin);

  if(switch_state == HIGH)
  {
    digitalWrite(LED_on, HIGH);
  }
  else
  {
    digitalWrite(LED_on, LOW);
  }
  
}
