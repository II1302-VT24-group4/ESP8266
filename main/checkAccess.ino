/**
 * @file checkAccess.ino
 * @brief Test program for development
 * @details Run this code by uncommenting the RUN_TEST_PROGRAM macro.
 */

/**
 * @brief Initializes the test environment.
 * 
 */
void createBooking(){
  String pathToIndex = "users/" + cardOwner + "/meetingIndex/" + currentDate;

  draw(cardOwner.c_str());
  delay(5000);

  if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", pathToIndex.c_str(), "", "")) {
  } else {
    draw("somthing went wrong :(");
    delay(5000);
  }
}

/**
 * @brief Initializes the test environment.
 * 
 */
bool checkAccess() {
  String pathToRFID = "test/" + uid + "/" + currentDate + "/" + currentMeetingID + "/rfid/" + cardParser();

  if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", pathToRFID.c_str(), "")) {

    FirebaseJson payload;
    payload.setJsonData(fbdo.payload().c_str());

    FirebaseJsonData jsonData;
    payload.get(jsonData, "fields/owner/stringValue", true);
    cardOwner = jsonData.stringValue;

    String pathToAttendees = "test/" + uid + "/" + currentDate + "/" + currentMeetingID + "/attendees/" + cardOwner;
    FirebaseJson content;
    content.set("fields/id/stringValue", cardOwner);

    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", pathToAttendees.c_str(), content.raw(), "id")) {
    }

    return true;

  } else {
    cardOwner = "";
    Serial.println(fbdo.errorReason());
  }

  return false;
}