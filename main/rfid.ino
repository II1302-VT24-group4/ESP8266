/**
 * @file rfid.ino
 * @brief Rfid code
 * @details Some of this code is taken from https://www.instructables.com/ and
 * modified by Christoffer Franzén
 */

/**
 * @brief Parses the RFID card string to the correct format.
 *
 * This function removes the start and end characters from the card number,
 * which are not needed when comparing the card ID with the database ID.
 *
 * @return The parsed card number.
 */
String cardParser() {
  String cardNumberNew = cardNumber.substring(1, cardNumber.length() - 1);
  return cardNumberNew;
}

/**
 * @brief Displays RFID data on the screen.
 *
 * This function retrieves the owner and email data associated with the RFID
 * card from Firestore, and displays this information on the screen. If the data
 * retrieval fails, it sounds a buzzer.
 */
void displayRFIDData() {
  String owner;
  String email;

  // Prepare the Firestore paths
  String rfidPath = "rfid/" + cardParser();
  String userPath;

  // Retrieve the owner data
  if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", rfidPath.c_str(),
                                     "")) {
    FirebaseJson payload;
    payload.setJsonData(fbdo.payload().c_str());

    FirebaseJsonData jsonData;
    payload.get(jsonData, "fields/owner/stringValue", true);

    owner = jsonData.stringValue;
    userPath = "users/" + owner;
  } else {
    draw("Did not find owner");
    delay(2000);
    return;
  }

  // Retrieve the email data
  if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", userPath.c_str(),
                                     "")) {
    FirebaseJson payload;
    payload.setJsonData(fbdo.payload().c_str());

    FirebaseJsonData jsonData;
    payload.get(jsonData, "fields/email/stringValue", true);

    email = jsonData.stringValue;
  } else {
    draw("Did not find email");
    delay(2000);
    return;
  }

  // Display the data
  u8g2.firstPage();
  do {
    delay(50);

    u8g2.setCursor(0, 12);
    u8g2.print(cardNumber.c_str());

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 22);
    u8g2.print(rfidPath.c_str());

    u8g2.setCursor(0, 34);
    u8g2.print(owner.c_str());

    u8g2.setCursor(0, 46);
    u8g2.print(email.c_str());
  } while (u8g2.nextPage());

  noTone(BUZZER);
}

/**
 * @brief Reads RFID data from the serial port.
 *
 * This function reads data from the serial port into a buffer. It then
 * updates the global cardNumber with the contents of the buffer and clears
 * the buffer for the next read.
 *
 * @return True if data was available and read, false otherwise.
 */
bool readRFIDData() {
  static char buffer[64];
  int count = 0;

  if (!SoftSerial.available()) {
    return false;
  }

  tone(BUZZER, 660);
  lastRFIDReadTime = millis();  // Update the time when data is received
  delay(50);                    // Wait for data to be ready
  noTone(BUZZER);

  while (SoftSerial.available() && count < sizeof(buffer)) {
    buffer[count++] = SoftSerial.read();
  }

  Serial.write(buffer, count);        // Echo the buffer for debugging
  cardNumber = String(buffer);        // Assuming cardNumber is a global String
  memset(buffer, 0, sizeof(buffer));  // Clear the buffer for the next read

  return true;
}

/**
 * @brief Detects the removal of an RFID card.
 *
 * This function checks if a certain amount of time has passed since the last
 * RFID read. If it has, it assumes that the card has been removed.
 *
 * @return True if the card is assumed to be removed, false otherwise.
 */
bool detectCardRemoval() {
  if (millis() - lastRFIDReadTime > removalTimeout) {
    return true;  // No data for more than 1 second, assume card removed
  }
  return false;  // Card is still present or not enough time has passed
}