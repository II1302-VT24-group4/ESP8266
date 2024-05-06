/**
 * @file stateMachine.ino
 * @brief
 * @details
 */

void setupStateMachine() {
  SoftSerial.begin(9600);  // the SoftSerial baud rate
  Serial.begin(9600);      // the Serial port of Arduino baud rate.

  // Screen
  u8g2.begin();           // Initialize the display
  u8g2.setContrast(100);  // Adjust the contrast level (0-255)

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

  // Print uid to console
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  delay(100);
  Serial.println(uid);
  delay(100);

  // Buttons
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_ABORT, INPUT_PULLUP);
}

void stateMachine() {
  unsigned long currentMillis = millis();

  updateTime();
  
  if (currentMillis - lastButtonUpdateTime >= buttonUpdateInterval) {
    lastButtonUpdateTime = currentMillis;
    uppdateButtons();
  }

  if (currentMillis - lastCalendarUpdateTime >= calendarUpdateInterval) {
    lastCalendarUpdateTime = currentMillis;
    updateDailyCalendar();

    nextAvailableTime = roomAvailable ? "" : nextFreeSlot(startTimes, endTimes);
  }

  switch (currentState) {
    case IDLE:
      drawIdle();

      if (button_confirm_state == LOW) {
        currentState = BOOK;
      }
      break;

    case BOOK:
      drawDefaultCalender();

      if (getButtonState() == "Abort") {
        currentState = IDLE;
      } else if (getButtonState() == "Down") {
        cursor = (cursor + 1) % 3;
      } else if (getButtonState() == "Up") {
        cursor = (cursor + 2) % 3;
      } else if (SoftSerial.available()) {
        currentState = RECEIVE_RFID_DATA;
      }
      break;

    case RECEIVE_RFID_DATA:
      if (readRFIDData()) {
        currentState = DISPLAY_CARD;
        cardPresent = true;
      }
      break;

    case DISPLAY_CARD:
      displayRFIDData();

      if (!SoftSerial.available() && cardPresent && detectCardRemoval()) {
        cardPresent = false;
      } else if (!cardPresent && SoftSerial.available() && readRFIDData() || getButtonState() == "Abort") {
        currentState = IDLE;
      }
      break;
  }
}