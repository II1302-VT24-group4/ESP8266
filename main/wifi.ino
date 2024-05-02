/**
 * @file wifi.ino
 * @brief WiFi connection function.
 * @details Some of this code is taken from https://randomnerdtutorials.com/
 */

/**
 * @brief Connects to a WiFi network.
 *
 * This function connects to a WiFi network using the provided SSID and
 * password. It prints a '.' every second until the connection is established,
 * then prints the local IP address.
 *
 * @param ssid The SSID of the WiFi network.
 * @param password The password of the WiFi network.
 */
void connectToWiFi(const char *ssid, const char *password) {
  Serial.begin(9600);
  WiFi.begin(ssid, password); // Connect to WiFi

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 30);
    u8g2.print("Connecting");
  } while (u8g2.nextPage());

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_RED, HIGH); 
    delay(1000);
    digitalWrite(LED_RED, LOW); 
    Serial.print("."); // while wifi not connected yet, print '.'
  }

  digitalWrite(LED_RED, LOW); 

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 30);
    u8g2.print("WiFi connected");
  } while (u8g2.nextPage());

  Serial.println(""); // new line, then print WiFi connected and the IP address
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}