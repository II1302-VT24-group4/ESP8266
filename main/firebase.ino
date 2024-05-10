/**
 * @file firebase.ino
 * @author Erik Heiskanen, Leo Andersson
 * @brief Firebase code.
 * @details This file contains functions related to Firebase operations.
 */

/**
 * @brief Updates the daily calendar.
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
 * @brief Updates the next available time slot.
 * 
 * @param startTimes An array containing start times.
 * @param endTimes An array containing end times.
 * @param sizeOfArray The size of the arrays.
 */
void updateNextAvailable(const String *startTimes, const String *endTimes, int sizeOfArray) {
  int counter = 0;

  // Check if counter is within the bounds of the array before accessing elements
  while (counter < sizeOfArray && startTimes[counter] != currentMeetingID) {
    counter++;
  }

  for (int i = counter; i < sizeOfArray - 1; i++) {
    if (endTimes[i] < startTimes[i + 1]) {
      // Removing the unused variable nextAvailableTimeSlot
      break;
    }
  }
}

/**
 * @brief Updates the next meeting time.
 * 
 * @param startTime An array containing start times.
 */
void updateNextMeeting(String *startTime) {
  int currentTime = (formattedTime.substring(0, 2) + formattedTime.substring(3, 6)).toInt();
  int i = 0;

  nextMeeting = "";

  while (startTime[i] != nullptr) {

    if (currentTime < (startTime[i].substring(0, 2) + startTime[i].substring(3, 6)).toInt())
      nextMeeting = startTime[i];

    i++;
  }
}