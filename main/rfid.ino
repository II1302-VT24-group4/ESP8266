/**
 * @file rfid.ino
 * @author Christoffer Franzén, Erik Heiskanen, Leo Andersson
 * @brief RFID functionality.
 * @details Handles RFID card parsing, data retrieval from Firestore, and display. 
 * Some of this code is taken from https://www.instructables.com/
 */

/**
 * @brief Parses the RFID card string.
 * 
 * Removes unnecessary start and end characters from the card number.
 * 
 * @return The parsed card number.
 */
String cardParser() {
  String cardNumberNew = cardNumber.substring(1, cardNumber.length() - 1);
  return cardNumberNew;
}

/**
 * @brief Displays RFID data.
 * 
 * Retrieves owner and email data associated with the RFID card from Firestore
 * and displays it on the screen.
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
 * Reads data from the serial port into a buffer and updates the global cardNumber.
 * 
 * @return True if data was read, false otherwise.
 */
bool readRFIDData() {
  static char buffer[64];
  int count = 0;

  if (!SoftSerial.available()) {
    return false;
  }

  tone(BUZZER, TONE_RFID);
  lastRFIDReadTime = millis();
  delay(50);
  noTone(BUZZER);

  while (SoftSerial.available() && count < sizeof(buffer)) {
    buffer[count++] = SoftSerial.read();
  }

  Serial.write(buffer, count);
  cardNumber = String(buffer);
  memset(buffer, 0, sizeof(buffer));  // Clear the buffer for the next read

  return true;
}