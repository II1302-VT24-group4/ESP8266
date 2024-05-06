/**
 * @file checkAccess.ino
 * @brief 
 * @details
 */

bool checkAccess(){
  String pathToRFID = "test/" + uid + "/" + currentDate + "/" + currentMeetingID + "/rfid";

    if(Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", pathToRFID.c_str(), "")) {

      return true;
   
  } else {
    Serial.println(fbdo.errorReason());
  }
    return false;
}