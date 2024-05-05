/**
 * @file test.ino
 * @brief Test program for development
 * @details Run this code by uncommenting the RUN_TEST_PROGRAM macro.
 */

/**
 * @brief Initializes the test environment.
 * 
 * This function initializes the necessary components and settings for testing.
 */
void setupTest(){
  SoftSerial.begin(9600); // the SoftSerial baud rate
  Serial.begin(9600);     // the Serial port of Arduino baud rate.

  // Screen
  u8g2.begin();          // Initialize the display
  u8g2.setContrast(100); // Adjust the contrast level (0-255)

  // LEDS
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);
  pinMode(BUZZER, INPUT_PULLUP);

  // Buttons
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_ABORT, INPUT_PULLUP);
}

/**
 * @brief Runs the test suite.
 * 
 * This function runs a series of tests to verify the functionality of different components.
 */
void test(){
  Test_BUTTONS();
  Test_GPIO();
  Test_BUZZER();

  draw("All tests done!");
  delay(2000);
}

/**
 * @brief Tests button functionality.
 * 
 * This function tests the functionality of buttons.
 * It checks if buttons are responsive and updates the display accordingly.
 */
void Test_BUTTONS(){
  draw("Buttons test click confirm to exit");
  bool exit = false;

  while(exit != true){
    uppdateButtons();
    if(button_confirm_state == LOW){
      exit = true;
    }

    delay(10);
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(0, 10, getButtonState().c_str());
      u8g2.drawStr(0, 30, String(buttons_direction).c_str());
    } while (u8g2.nextPage());
    delay(10);
  }
}

/**
 * @brief Tests GPIO functionality.
 * 
 * This function tests the functionality of GPIO pins, specifically LED blinking.
 * It checks if LEDs are blinking and updates the display accordingly.
 */
void Test_GPIO(){
  draw("Are the red and green LED blinking?");
  bool exit = false;

  while(exit != true){
    uppdateButtons();
    if(button_confirm_state == LOW){
      exit = true;
    }

    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, HIGH);
    delay(1000);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    delay(1000);
  }

  draw("GPIO test done!");
  delay(2000);
}

/**
 * @brief Tests buzzer functionality.
 * 
 * This function tests the functionality of the buzzer.
 * It checks if the buzzer is making noise and updates the display accordingly.
 */
void Test_BUZZER(){
  draw("Is the buzzer making noise?");

  bool exit = false;

  while(exit != true){
    uppdateButtons();
    if(button_confirm_state == LOW){
      exit = true;
    }

    tone(BUZZER, 550);
    delay(1000);
    noTone(BUZZER);
    delay(1000);
  }

  draw("BUZZER test done!");
  delay(2000);
}