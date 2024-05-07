/**
 * @file test.ino
 * @brief Test program for development
 * @details Run this code by uncommenting the RUN_TEST_PROGRAM macro.
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

  String pathToUser = "users/9rsVJfyNVmU2uneewX8PQCTz5Tw2";
  
  FirebaseJson contentTest;

  contentTest.set("fields/meetings/stringValue", currentDate);

  if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", pathToUser.c_str(), contentTest.raw(), "meetings")) {
    draw("Set chnage to firebase");
    delay(5000);
  } else {
    draw(fbdo.errorReason().c_str());
    delay(5000);
  }

  return false;
}