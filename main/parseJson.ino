bool parseJson(String jsonString, String currentTime) {
  for (int i = 0; i < 48; i++) {
    startTimes[i] = "";
    endTimes[i] = "";
  }

  documentsCount = 0;
  const size_t capacity = JSON_OBJECT_SIZE(5) + 190;
  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, jsonString);

  // Loopa genom varje dokument
  for (JsonObject obj : doc["documents"].as<JsonArray>()) {
    const char* startTime = obj["fields"]["startTime"]["stringValue"];
    const char* endTime = obj["fields"]["endTime"]["stringValue"];

    // Spara starttiden och sluttiden i respektive array
    startTimes[documentsCount] = startTime;
    endTimes[documentsCount] = endTime;

    documentsCount++;

    // För att undvika att överskrida arrayens gränser
    if (documentsCount >= MAX_DOCUMENTS) {
      break;
    }
  }
  bool result = true;
  for (int i = 0; i < documentsCount; i++) {
    Serial.println((currentTime.substring(0, 2) + currentTime.substring(3, 6)).toInt());
    Serial.println((startTimes[i].substring(0, 2) + startTimes[i].substring(3, 6)).toInt());
    Serial.println((currentTime.substring(0, 2) + currentTime.substring(3, 6)).toInt() < (startTimes[i].substring(0, 2) + startTimes[i].substring(3, 6)).toInt());

    if ((currentTime.substring(0, 2) + currentTime.substring(3, 6)).toInt() < (startTimes[i].substring(0, 2) + startTimes[i].substring(3, 6)).toInt())
      return result;

    result = timeInRange(currentTime, startTimes[i], endTimes[i]);
  }

  return result;
}