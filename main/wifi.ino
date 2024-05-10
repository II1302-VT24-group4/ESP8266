/**
 * @file wifi.ino
 * @author Christoffer Franzén
 * @brief WiFi connection function.
 * @details Code for connecting to a WiFi network.
 * Some portions of this code are adapted from https://randomnerdtutorials.com/
 */

/**
 * @brief Connects to a WiFi network.
 *
 * Connects to a WiFi network using the provided SSID and password. Prints
 * connection status and local IP address upon successful connection.
 *
 * @param ssid SSID of the WiFi network.
 * @param password Password of the WiFi network.
 */
void connectToWiFi(const char *ssid, const char *password) {
  Serial.begin(9600);
  WiFi.begin(ssid, password);  // Connect to WiFi

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startTime > 30000) {  // 30 seconds timeout
      Serial.println("Failed to connect to WiFi.");
      break;
    }

    digitalWrite(LED_RED, HIGH);

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.setCursor(0, 20);
      u8g2.print("Connecting");
      u8g2.setCursor(0, 40);
      u8g2.print(ssid);
    } while (u8g2.nextPage());

    delay(1000);
    digitalWrite(LED_RED, LOW);

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.setCursor(0, 20);
      u8g2.print("Connecting.");
      u8g2.setCursor(0, 40);
      u8g2.print(ssid);
    } while (u8g2.nextPage());

    Serial.print(".");  // while wifi not connected yet, print '.'
  }

  digitalWrite(LED_RED, LOW);

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 20);
    u8g2.print("WiFi connected");
    u8g2.setCursor(0, 40);
    u8g2.print(ssid);
  } while (u8g2.nextPage());

  Serial.println("");  // new line, then print WiFi connected and the IP address
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}