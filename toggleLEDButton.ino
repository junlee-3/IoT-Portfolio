const int BUTTON_PIN = 4; 
const int LED_PIN    = 16; 

// variables will change:
int ledState = LOW;     // the current state of LED
int lastButtonState;    // the previous state of button
int currentButtonState; 

void setup() {
  Serial.begin(9600);                
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(LED_PIN, OUTPUT);          

  currentButtonState = digitalRead(BUTTON_PIN);
}

void loop() {
  lastButtonState    = currentButtonState;      
  currentButtonState = digitalRead(BUTTON_PIN); 

  if(lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("The button is pressed");

    // toggle state of LED
    ledState = !ledState;

    // control LED arccoding to the toggled state
    digitalWrite(LED_PIN, ledState); 
  }
}
