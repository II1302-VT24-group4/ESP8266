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

void updateDailyCalendar(){
  String pathToMeetings = "test/" + uid + "/" + currentDate;
  Serial.println(pathToMeetings);
  
  if(Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", pathToMeetings.c_str(), "")){
    // Create a FirebaseJson object and set content with received payload
    Serial.println("Före payload");
    FirebaseJson payload;
    Serial.println("Efter payload");
    payload.setJsonData(fbdo.payload().c_str());
    Serial.println(fbdo.payload().c_str());
    Serial.println("Efter SetJson");
    delay(100);
    
    String jsonString = fbdo.payload().c_str();
    bool available = parseJson(jsonString, formattedTime);
    Serial.println(available);
    
    if(available){
    String path2 = "test/" + uid;

    // Create document to send to firebase
    FirebaseJson content;
    content.set("fields/available/booleanValue", true);

      if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", path2.c_str(), content.raw(), "available")) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          roomAvailable = true;

          
      } else {
          Serial.println(fbdo.errorReason());
      }
    } else {
      String path2 = "test/" + uid;

      // Create document to send to firebase
      FirebaseJson content;
      content.set("fields/available/booleanValue", false);

      if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", path2.c_str(), content.raw(), "available")) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          roomAvailable = false;

          
      } else {
          Serial.println(fbdo.errorReason());
      }

    }

  } else {
    String path2 = "test/" + uid;

    // Create document to send to firebase
    FirebaseJson content;
    content.set("fields/available/booleanValue", true);

      if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", path2.c_str(), content.raw(), "available")) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());

          
      } else {
        Serial.println(fbdo.errorReason());
      }
    roomAvailable = true;
    currentMeetingID = "";  
  }
}


void updateNextAvailable(String *startTimes, String *endTimes, int sizeOfArray) {
  int counter = 0;
  while(startTimes[counter] != currentMeetingID){
    counter++;
  }
  
  for(int i = counter; i < sizeOfArray; i++){
    if(endTimes[counter] < startTimes[counter + 1]){
      String nextAvailableTimeSlot = endTimes[counter] + " - " + startTimes[counter + 1];
      break;
    }
    if(counter >= sizeOfArray)
      break;

  }
}