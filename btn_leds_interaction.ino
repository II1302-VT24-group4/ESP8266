// LEDS
int LED_RED = 15; //GPIO 15
int LED_GREEN = 16; // GPIO 16
// Buttons
int BUTTON1 = 13; // GPIO 13
int BUTTON2 = 12; // GPIO 13
int BUTTON3 = 14; // GPIO 14
int BUTTON4 = 2; // GPIO 2

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  // LED
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  // Button
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
  if((digitalRead(BUTTON1) == LOW)){
    digitalWrite(LED_GREEN, HIGH);
  } 
  if(digitalRead(BUTTON2) == LOW){
    digitalWrite(LED_GREEN, LOW);
  }
  if(digitalRead(BUTTON3) == LOW){
    digitalWrite(LED_RED, HIGH);
  }
  if(digitalRead(BUTTON4) == LOW){
    digitalWrite(LED_RED, LOW);
  }
}