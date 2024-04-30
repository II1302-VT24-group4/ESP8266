/**
 * @file wifi.ino
 * @brief WiFi connection function.
 * @details This code is taken from https://randomnerdtutorials.com/
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

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("."); // while wifi not connected yet, print '.'
  }

  Serial.println(""); // new line, then print WiFi connected and the IP address
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}