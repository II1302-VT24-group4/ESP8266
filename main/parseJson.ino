/**
 * @file parseJson.ino
 * @author Erik Heiskanen, Leo Andersson
 * @brief JSON parsing functionality.
 * @details Parses JSON data to extract start and end times for documents.
 */

/**
 * @brief Parses JSON data to extract start and end times.
 * 
 * Parses the provided JSON string to extract start and end times for documents.
 * 
 * @param jsonString The JSON string to parse.
 * @param currentTime The current time used for comparison.
 * @return True if parsing is successful, false otherwise.
 */
bool parseJson(String jsonString, String currentTime) {
  for (int i = 0; i < 48; i++) {
    startTimes[i] = "";
    endTimes[i] = "";
  }

  documentsCount = 0;
  //const size_t capacity = JSON_OBJECT_SIZE(5) + 190;
  const size_t capacity = 190;
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

void parseFirestoreData(String json) {
    FirebaseJson data;
    FirebaseJsonData jsonData;

    data.setJsonData(json);

    if (data.get(jsonData, "documents", FirebaseJson::JSON_ARRAY)) {
        FirebaseJsonArray documentsArray;
        jsonData.getArray(documentsArray);

        for (size_t i = 0; i < documentsArray.size(); i++) {
            FirebaseJsonData docData;
            if (documentsArray.get(docData, i)) {
                FirebaseJson docJson;
                docJson.setJsonData(docData.stringValue);

                String test, name, available;
                if (docJson.get(jsonData, "fields/test/stringValue")) {
                    test = jsonData.stringValue;
                }

                if (docJson.get(jsonData, "fields/available/booleanValue")) {
                    available = jsonData.boolValue ? "Yes" : "No";
                }

                // Display on the LCD
                u8g2.firstPage();
                do {
                    u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a suitable font
                    u8g2.setCursor(0, 15);
                    u8g2.print("Room: ");
                    u8g2.print(test);
                    u8g2.setCursor(0, 30);
                    u8g2.print("Available: ");
                    u8g2.print(available);
                } while (u8g2.nextPage());
            }
        }
    }
} 
