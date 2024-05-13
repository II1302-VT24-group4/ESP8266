/**
 * @file stateMachine.ino
 * @author Christoffer Franzén, Erik Heiskanen, Leo Andersson, Hein Lee
 * @brief Manages the state machine for the system.
 * @details Initializes components and handles system state transitions.
 */

/**
 * @brief Sets up the state machine.
 * 
 * Initializes components such as screen, LEDs, buttons, and connects to WiFi.
 */
void setupStateMachine() {
  SoftSerial.begin(9600);
  Serial.begin(9600);

  // Screen
  u8g2.begin();
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

  // Initialize NTPClient
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
  delay(50);
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(500);
  }

  // Print uid to console
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  delay(50);
  Serial.println(uid);
  delay(50);

  // Buttons
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_ABORT, INPUT_PULLUP);

}

/**
 * @brief Manages the system state transitions.
 */
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
      updateNextMeeting(startTimes);

      drawIdle();

      if (getButtonState() == "Left") {
        currentState = NEXTROOM;
      } else if (getButtonState() == "Right") {
        currentState = NEXTROOM;
      }

      if (SoftSerial.available() && nextAvailableTime.isEmpty()) {
        readRFIDData();
        String rfidPath = "rfid/" + cardParser();
        // Retrieve the owner data
        if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", rfidPath.c_str(), "")) {
          FirebaseJson payload;
          payload.setJsonData(fbdo.payload().c_str());

          FirebaseJsonData jsonData;
          payload.get(jsonData, "fields/owner/stringValue", true);
          cardOwner = jsonData.stringValue;


          currentState = QUICKBOOK;
        } else {
          draw("Card not registered!");
          delay(3000);
          currentState = IDLE;
        }

      } else if (SoftSerial.available() && !nextAvailableTime.isEmpty()) {
        readRFIDData();

        if (checkAccess()) {
          //draw("Room unlocked!");
          //delay(2000);

          u8g2.firstPage();
          do {
            drawUnlockedLockIcon();
            u8g2.setFont(u8g2_font_unifont_t_symbols);
            u8g2.drawStr(20, 62, "Room unlocked!");
          } while (u8g2.nextPage());

          delay(4000);
        } else {
          // Prepare the Firestore paths


          String rfidPath = "rfid/" + cardParser();
          // Retrieve the owner data
          if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", rfidPath.c_str(), "")) {
            FirebaseJson payload;
            payload.setJsonData(fbdo.payload().c_str());

            FirebaseJsonData jsonData;
            payload.get(jsonData, "fields/owner/stringValue", true);
            cardOwner = jsonData.stringValue;


            currentState = QUICKBOOK;

          } else {
            draw("Card not registered!");
            delay(3000);
          }
        }
      }
      break;

    case QUICKBOOK:

      if (roomAvailable) {


        int currentTime = (formattedTime.substring(0, 2) + formattedTime.substring(3, 6)).toInt();
        int nextMeetingTime = (nextMeeting.substring(0, 2) + nextMeeting.substring(3, 6)).toInt();
        int timeDiff = nextMeetingTime - currentTime;
        String startTime = "";
        String endTime = "";


        if (formattedTime.substring(3, 6).toInt() <= 30) {
          startTime = formattedTime.substring(0, 2) + ":00";
        } else {
          startTime = formattedTime.substring(0, 2) + ":30";
        }
        if (timeDiff < 100 && nextMeeting != nullptr) {
          endTime = nextMeeting;
          quickBookType = 2;
        } else {
          endTime = String(startTime.substring(0, 2).toInt() + 1) + ":" + startTime.substring(3, 6);
          quickBookType = 2;
        }

        String booking = startTime + " - " + endTime;

        drawConfirmBooking(booking);
      } else {
        String startTime;
        String endTime;
        if (nextAvailableTime.length() == 8) {

          startTime = nextAvailableTime.substring(0, 5);

          endTime = String(nextAvailableTime.substring(0, 3).toInt() + 1) + ":" + nextAvailableTime.substring(3, 5);

          String booking = startTime + " - " + endTime;

          drawConfirmBooking(booking);

        } else {
          // Om bokning finns skapa bokning vid nästa lediga tid
          startTime = nextAvailableTime.substring(0, 5);
          int startOfIntervall = (nextAvailableTime.substring(0, 2) + nextAvailableTime.substring(3, 6)).toInt();
          int endOfIntervall = (nextAvailableTime.substring(8, 10) + nextAvailableTime.substring(11, 14)).toInt();
          int timeDifference = endOfIntervall - startOfIntervall;

          if (timeDifference >= 100) {
            endTime = String(nextAvailableTime.substring(0, 3).toInt() + 1) + ":" + nextAvailableTime.substring(3, 5);

            String booking = startTime + " - " + endTime;

            drawConfirmBooking(booking);

          } else if (timeDifference == 70 || timeDifference == 30) {
            quickBookType = 1;

            drawConfirmBooking(nextAvailableTime);
          }
        }
      }

      if (getButtonState() == "Abort") {
        quickBookType = 0;
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
      break;

    case CONFIRMQUICKBOOK:  // Logic for confirming quick booking and creating it
      createBooking();

      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, formattedTime.c_str());
        u8g2.drawStr(0, 20, "Booking created");
      } while (u8g2.nextPage());

      delay(3000);

      currentState = IDLE;

      break;

    case NEXTROOM:
      
        /* String text3 = "Check other ";
        String text4 = "Rooms status";
        String displayTexts[] = {formattedTime.substring(0, formattedTime.length() - 3), text3, text4}; */
        
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.firstPage();
        do {
          u8g2.drawStr(0, 10, formattedTime.c_str());
          u8g2.drawStr(0, 20, "Check other");
          u8g2.drawStr(0, 30, "Rooms status");
        } while (u8g2.nextPage());

        if (getButtonState() == "Confirm" ) {
          currentState = IDLE;
        } else if (getButtonState() == "Up") {
          //cursor = (cursor + 1) % 3;
          currentState = OTHERROOM;
        } else if (getButtonState() == "Down") {
          //cursor = (cursor + 2) % 3;
          currentState = OTHERROOM;
        } else if (getButtonState() == "Abort" ) {
          currentState = IDLE;
        }
      break;


    case OTHERROOM: 
    if (getButtonState() == "Abort") {
    currentState = IDLE;
    break; // Exit the case immediately if aborting.
    }
    fetchData();   
  break; 
  }
}