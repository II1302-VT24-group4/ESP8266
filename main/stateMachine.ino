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

      if (SoftSerial.available() && nextAvailableTime.isEmpty()) {
        readRFIDData();
        currentState = QUICKBOOK;
      } else if (SoftSerial.available() && !nextAvailableTime.isEmpty()) {
        readRFIDData();

        if (checkAccess()) {
          draw("Room unlocked!");
          delay(1000);

          u8g2.firstPage();
          do {
            u8g2.setFont(u8g2_font_ncenB08_tr); // Set the font

            // Draw a larger and more detailed unlocked icon
            u8g2.drawFrame(34, 12, 60, 40); // Outer rectangle
            u8g2.drawBox(38, 22, 52, 26); // Inner rectangle
            u8g2.drawCircle(64, 22, 10); // Circle
            u8g2.drawBox(64, 12, 8, 10); // Rectangle on top of the circle

          } while (u8g2.nextPage());

          delay(3000);
        } else {
          // Prepare the Firestore paths
          String rfidPath = "rfid/" + cardParser();
          String userPath;

          // Retrieve the owner data
          if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", rfidPath.c_str(),
                                             "")) {
            

            currentState = QUICKBOOK;

          } else {
            draw("Card not registerd");
            delay(3000);
          }
        }
      }
      break;

    case QUICKBOOK:

      if(roomAvailable){
        draw("roomAvailable :) ");
        tone(BUZZER, 330);
        delay(50);
        noTone(BUZZER);
        delay(1450);
      } else {  // Om bokning finns skapa bokning vid nästa lediga tid
        u8g2.firstPage();
        do {
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, cardNumber.c_str());
          u8g2.drawStr(0, 20, "Do you want too book:");
          u8g2.drawStr(0, 30, nextAvailableTime.c_str());
        } while (u8g2.nextPage());

        if (getButtonState() == "Abort") {
          draw("Booking aborted");
          tone(BUZZER, TONE_ABORT);
          delay(50);
          noTone(BUZZER);
          delay(1450);
          currentState = IDLE;
        } else if (getButtonState() == "Confirm") {
          tone(BUZZER, TONE_CONFIRM);
          delay(50);
          noTone(BUZZER);
          currentState = CONFIRMQUICKBOOK;
        }
      }




      break;

    case CONFIRMQUICKBOOK:
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, formattedTime.c_str());
        u8g2.drawStr(0, 20, "Booking created");
      } while (u8g2.nextPage());

      delay(3000);

      currentState = IDLE;

      break;

    case BOOK:
      drawDefaultCalender();

      if (getButtonState() == "Abort") {
        currentState = IDLE;
      } else if (getButtonState() == "Down") {
        cursor = (cursor + 1) % 3;
      } else if (getButtonState() == "Up") {
        cursor = (cursor + 2) % 3;
      }
      break;
  }
}