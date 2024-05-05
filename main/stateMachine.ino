/**
 * @file stateMachine.ino
 * @brief
 * @details
 */

void setupStateMachine(){
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

  // WiFI
  for (int i = 0; i < sizeof(ssids) / sizeof(ssids[0]); i++) {
    connectToWiFi(ssids[i], passwords[i]);
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
  }

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(7200);

  // Set up config API KEY
  config.api_key = API_KEY;

  // Set up firebase auth
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Generate token
  config.token_status_callback = tokenStatusCallback;

  // Connect to firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Get userID
  Serial.println("Getting User UID");
  delay(500);
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(500);
  }

  // Buttons
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_ABORT, INPUT_PULLUP);

}

void stateMachine(){
  unsigned long currentMillis = millis();
  updateTime();

  // Update buttons at regular intervals
  if (currentMillis - lastButtonUpdateTime >= buttonUpdateInterval) {
    uppdateButtons();
    lastButtonUpdateTime = currentMillis;
  }

  if (currentMillis - lastCalendarUpdateTime >= calendarUpdateInterval) {
    lastCalendarUpdateTime = currentMillis; 

    updateDailyCalendar();

    if (roomAvailable == false) {
      nextAvailableTime = nextFreeSlot(startTimes, endTimes);
    } else {
      nextAvailableTime = "";
    }

  }

  switch (currentState) {
  case IDLE:
    drawIdle();
    
    // Om användaren trycker på någon knapp
    if(button_confirm_state == LOW || button_abort_state == LOW){
      currentState = BOOK;
    }
  break;

  case BOOK:
    drawDefaultCalender();

    static unsigned long lastButtonPressTime = 0;

    // Handle button press
    if (getButtonState() != "None" && millis() - lastButtonPressTime >= 150) {
        lastButtonPressTime = millis();

        if (getButtonState() == "Abort") {
            currentState = IDLE;
        } else if (getButtonState() == "Down") {
            cursor = (cursor + 1) % 3;
        } else if (getButtonState() == "Up") {
            cursor = (cursor - 1 + 3) % 3;
        }
    }

    // Handle RFID data
    if (SoftSerial.available()) {
        currentState = RECEIVE_RFID_DATA;
    }
    break;

  case RECEIVE_RFID_DATA:
    if (readRFIDData()) {
      currentState = DISPLAY_CARD;
      cardPresent = true; // Set card presence flag
    }
    break;

  case DISPLAY_CARD:
    displayRFIDData();
    if (!SoftSerial.available() && cardPresent) {
      if (detectCardRemoval()) {
        cardPresent = false; // Card is removed
      }
    } else if (!cardPresent && SoftSerial.available()) {
      // Detect fresh card presentation
      if (readRFIDData()) { // Make sure it's a valid card re-presentation
        currentState =
            IDLE; // Transition back to IDLE on second card presentation
      }
    }
    break;
  }
}