// GPIO 2 (D5) has a LED_BLINK attached to it. Give it a name:
int LED_GREEN = 13; // GPIO 13, D2
int LED_RED = 10; //GPIO 10, D3
// Buttons
int BUTTON1 = 15; // GPIO 15, D4
int BUTTON2 = 2; // GPIO 2, D5
int BUTTON3 = 5; // GPIO 5, D6
int BUTTON4 = 4; // GPIO 4, D7
int lastState = HIGH;
int currentState;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
 // digitalWrite(LED_GREEN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //digitalWrite(LED_RED, LOW);
  //delay(100);                       // wait for a second
  //digitalWrite(LED_GREEN, LOW);    // turn the LED off by making the voltage LOW
  //digitalWrite(LED_RED, HIGH);
  //delay(100);                       // wait for a second
  
  if(digitalRead(BUTTON1) == HIGH){ 
    //Serial.println("btn1");
    digitalWrite(LED_GREEN, LOW);
  }
  else if(digitalRead(BUTTON2) == LOW){
    //Serial.println("btn2");
    digitalWrite(LED_GREEN, HIGH);
  }
  else if(digitalRead(BUTTON3) == LOW){
    //Serial.println("btn3");
    digitalWrite(LED_GREEN, HIGH);
  }
  else if(digitalRead(BUTTON4) == LOW){ 
    //Serial.println("btn4");
    digitalWrite(LED_GREEN, HIGH);
  }else{
    digitalWrite(LED_GREEN, LOW);
  }
  
  
}