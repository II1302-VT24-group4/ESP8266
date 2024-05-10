/**
 * @file test.ino
 * @author Christoffer Franzén
 * @brief Test program for development
 * @details Run this code by uncommenting the RUN_TEST_PROGRAM macro.
 */

/**
 * @brief Initializes the test environment.
 * 
 * This function initializes the necessary components and settings for testing.
 */
void setupTest() {
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

  // Buttons
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_ABORT, INPUT_PULLUP);
}

/**
 * @brief Runs the test suite.
 * 
 * This function runs a series of tests to verify the functionality of different components.
 */
void test() {
  testButtons();
  testGPIO();
  testBuzzer();
  testWifi();
  testRFID();
  testNextRoom();

  draw("All tests done!");
  Serial.write("All tests done!\n");
  delay(4000);
}

/**
 * @brief Tests button functionality.
 * 
 * This function tests the functionality of buttons.
 * It checks if buttons are responsive and updates the display accordingly.
 */
void testButtons() {
  draw("Buttons test click confirm to exit");
  bool exit = false;

  while (!exit) {
    uppdateButtons();

    if (button_confirm_state == LOW) {
      exit = true;
    }

    delay(100);

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(0, 10, getButtonState().c_str());
      u8g2.drawStr(0, 30, String(buttons_direction).c_str());
    } while (u8g2.nextPage());

    delay(100);
  }
}

/**
 * @brief Tests GPIO functionality.
 * 
 * This function tests the functionality of GPIO pins, specifically LED blinking.
 * It checks if LEDs are blinking and updates the display accordingly.
 */
void testGPIO() {
  draw("Are the red and green LED blinking?");
  bool exit = false;

  delay(200);

  while (!exit) {
    uppdateButtons();
    if (button_confirm_state == LOW) {
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
void testBuzzer() {
  draw("Is the buzzer making noise?");

  bool exit = false;

  while (!exit) {
    uppdateButtons();
    if (button_confirm_state == LOW) {
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

/**
 * @brief Tests WiFi functionality.
 * 
 * This function tests the WiFi connection.
 * It attempts to connect to WiFi and displays the result on the screen.
 */
void testWifi() {
  draw("Connecting to WiFi...");

  // Attempt to connect to WiFi
  WiFi.begin(ssids[0], passwords[0]);

  // Display connecting message
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, "Connecting to:");
    u8g2.drawStr(0, 30, ssids[0]);
  } while (u8g2.nextPage());

  // Wait for WiFi connection
  int connectionAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && connectionAttempts < 20) {
    delay(500);
    connectionAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    // Display connection success message with IP address
    String ip = ipToString(WiFi.localIP());
    draw("Connected to WiFi");
    delay(1000);
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(0, 10, "Connected to:");
      u8g2.drawStr(0, 20, ssids[0]);
      u8g2.drawStr(0, 40, "IP Address:");
      u8g2.drawStr(0, 50, ip.c_str());
    } while (u8g2.nextPage());
  } else {
    // Display connection failure message
    draw("Failed to connect to WiFi");
    delay(2000);
  }

  delay(5000);
  draw("WiFi test done!");
  delay(2000);
}

/**
 * @brief Tests RFID functionality.
 * 
 * This function tests the RFID reader.
 * It waits for a card to be detected and displays its number.
 */
void testRFID() {
  bool exit = false;
  draw("RFID test, blip a card");

  while (!exit) {
    if (SoftSerial.available()) {
      draw("Reading card...");
      readRFIDData();
      delay(1000);

      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, cardNumber.c_str());
      } while (u8g2.nextPage());
      delay(6000);

      draw("RFID test done!");
      delay(5000);
      exit = true;
    }
  }
}

void testNextRoom() {
}

/**
 * @brief Converts IP address to string format.
 * 
 * @param ip The IP address to convert.
 * @return String The IP address in string format.
 */
String ipToString(const IPAddress& ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}