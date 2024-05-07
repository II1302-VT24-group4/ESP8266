/**
 * @file test.ino
 * @brief Test program for development
 * @details Run this code by uncommenting the RUN_TEST_PROGRAM macro.
 */

bool checkAccess(){
  String pathToRFID = "test/" + uid + "/" + currentDate + "/" + currentMeetingID + "/rfid/" + cardParser();

    if(Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", pathToRFID.c_str(), "")) {

      FirebaseJson payload;
      payload.setJsonData(fbdo.payload().c_str());

      FirebaseJsonData jsonData;
      payload.get(jsonData, "fields/owner/stringValue", true);
      String cardOwner = jsonData.stringValue;

      String pathToAttendees = "test/" + uid + "/" + currentDate + "/" + currentMeetingID + "/attendees/" + cardOwner;
      FirebaseJson content;
      content.set("fields/id/stringValue", cardOwner);

      if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", pathToAttendees.c_str(), content.raw(), "id")) {
      
      }

      return true;
   
  } else {
    Serial.println(fbdo.errorReason());
  }
    return false;
}