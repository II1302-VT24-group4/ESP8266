/**
 * @file firebase.ino
 * @brief Firebase code
 * @details
 */

/*
 * The FirebaseJsonData object holds the returned data which can be read from
 * the following properties. jsonData.stringValue - contains the returned
 * string. jsonData.intValue - contains the returned integer value.
 * jsonData.floatValue - contains the returned float value.
 * jsonData.doubleValue - contains the returned double value.
 * jsonData.boolValue - contains the returned boolean value.
 * jsonData.success - used to determine the result of the get operation.
 * jsonData.type - used to determine the type of returned value in string
 * represent the types of value e.g. string, int, double, boolean, array,
 * object, null and undefined.
 */

/**
 * @brief
 * 
 */
void updateDailyCalendar() {
  String pathToMeetings = "test/" + uid + "/" + currentDate;

  if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", pathToMeetings.c_str(), "")) {
    FirebaseJson payload;
    payload.setJsonData(fbdo.payload().c_str());

    String jsonString = fbdo.payload().c_str();
    bool available = parseJson(jsonString, formattedTime);

    String path2 = "test/" + uid;
    FirebaseJson content;
    content.set("fields/available/booleanValue", available);

    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", path2.c_str(), content.raw(), "available")) {
      roomAvailable = available;
    } else {
      Serial.println(fbdo.errorReason());
    }
  } else {
    String path2 = "test/" + uid;
    FirebaseJson content;
    content.set("fields/available/booleanValue", true);

    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", path2.c_str(), content.raw(), "available")) {
      roomAvailable = true;
      currentMeetingID = "";
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
}

/**
 * @brief
 * 
 */
void updateNextAvailable(String *startTimes, String *endTimes, int sizeOfArray) {
  int counter = 0;
  while (startTimes[counter] != currentMeetingID && counter < sizeOfArray) {
    counter++;
  }

  for (int i = counter; i < sizeOfArray - 1; i++) {
    if (endTimes[i] < startTimes[i + 1]) {
      String nextAvailableTimeSlot = endTimes[i] + " - " + startTimes[i + 1];
      break;
    }
  }
}