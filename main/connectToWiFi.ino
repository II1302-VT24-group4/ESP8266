void connectToWiFi(const char* ssid, const char* password) {
  Serial.begin(9600);
  WiFi.begin(ssid, password);   // Connect to WiFi
  pinMode(D4, OUTPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");   // while wifi not connected yet, print '.'
  }

  Serial.println("");  //print a new line, then print WiFi connected and the IP address
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}
