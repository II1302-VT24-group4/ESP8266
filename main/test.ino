/**
 * @file test.ino
 * @brief Test program for development
 * @details Run this code by uncomment the RUN_TEST_PROGRAM.
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

  // WiFI
  //for (int i = 0; i < sizeof(ssids) / sizeof(ssids[0]); i++) {
  //  connectToWiFi(ssids[i], passwords[i]);
  //  if (WiFi.status() == WL_CONNECTED) {
  //    break;
  //  }
  //}

  // Initialize a NTPClient to get time
  //timeClient.begin();
  //timeClient.setTimeOffset(7200);

  // Set up config API KEY
  //config.api_key = API_KEY;

  // Set up firebase auth
  //auth.user.email = USER_EMAIL;
  //auth.user.password = USER_PASSWORD;

  // Generate token
  //config.token_status_callback = tokenStatusCallback;

  // Connect to firebase
  //Firebase.begin(&config, &auth);
  //Firebase.reconnectWiFi(true);

  // Get userID
  //Serial.println("Getting User UID");
  //delay(500);
  //while ((auth.token.uid) == "") {
  //  Serial.print('.');
  //  delay(500);
  //}

  // Print uid to console
  //uid = auth.token.uid.c_str();
  //Serial.print("User UID: ");
  //delay(500);
  //Serial.println(uid);
  //delay(500);
}

void test(){
  Test_BUTTONS();
  Test_GPIO();
  Test_BUZZER();

  draw("All test done!");
  delay(2000);
}

void Test_BUTTONS(){
  draw("Buttons test click confirm to exit");
  bool exit = false;

  while(exit != true){
    uppdateButtons();
    if(button_confirm_state == LOW){
      exit = true;
    }

    delay(1000);
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(0, 10, getButtonState().c_str());
      u8g2.drawStr(0, 30, String(buttons_direction).c_str());
    } while (u8g2.nextPage());
    delay(1000);
  }
}

void Test_GPIO(){
  draw("Are the red and green led blinking?");
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

void Test_BUZZER(){
  draw("Are the buzzer making noise?");

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
