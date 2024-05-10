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

  


 

  String pathToUserIndex = "users/" + cardOwner + "/meetingIndex/" + currentDate;


  

  if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", pathToUserIndex.c_str(), "", "")) {
  } else {
    draw("something went wrong :(");
    delay(5000);
  }

  String pathToRoomIndex = "test/" + uid + "/meetingIndex/" + currentDate;

  if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", pathToRoomIndex.c_str(), "", "")) {
  } else {
    draw("something went wrong :(");
    delay(5000);
  }
  
  if(nextAvailableTime.length() == 8 || quickBookType == 0){
    
    FirebaseJson content;
    String startTime = nextAvailableTime.substring(0, 5);
    content.set("fields/startTime/stringValue", startTime);
    String endTime = String(nextAvailableTime.substring(0, 3).toInt() + 1) + ":" + nextAvailableTime.substring(3, 5);
    content.set("fields/endTime/stringValue", endTime);
    content.set("fields/owner/stringValue", cardOwner);
    content.set("fields/title/stringValue", "quick booking");

    String toSaveBookingForRoom = "test/" + uid + "/" + currentDate + "/" + startTime;

    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toSaveBookingForRoom.c_str(), content.raw(), "")) {
    } else {
      draw("something went wrong :(");
      delay(5000);
    }

    String toSaveBookingForUser = "users/" + cardOwner + "/" + currentDate + "/" + startTime;
    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toSaveBookingForUser.c_str(), content.raw(), "")) {
    } else {
      draw("something went wrong :(");
      delay(5000);
    }

    String toRfidAccess = toSaveBookingForRoom + "/rfid/" + cardParser();

    FirebaseJson rfidContent;
    rfidContent.set("fields/owner/stringValue", cardOwner);

    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toRfidAccess.c_str(), rfidContent.raw(), "")) {
  } else {
    draw("something went wrong :(");
    delay(5000);
  }

    
    /*draw(startTime.c_str());
    delay(3000);
    draw(endTime.c_str());
    delay(3000);*/
  } else if(quickBookType == 1){
    FirebaseJson content;
    String startTime = nextAvailableTime.substring(0, 5);
    content.set("fields/startTime/stringValue", startTime);
    String endTime = nextAvailableTime.substring(8, 14);
    content.set("fields/endTime/stringValue", endTime);
    content.set("fields/owner/stringValue", cardOwner);
    content.set("fields/title/stringValue", "quick booking");
    
    
    String toSaveBookingForRoom = "test/" + uid + "/" + currentDate + "/" + startTime;

    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toSaveBookingForRoom.c_str(), content.raw(), "")) {
    } else {
      draw("something went wrong :(");
      delay(5000);
    }

    String toSaveBookingForUser = "users/" + cardOwner + "/" + currentDate + "/" + startTime;
    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toSaveBookingForUser.c_str(), content.raw(), "")) {
    } else {
      draw("something went wrong :(");
      delay(5000);
    }

    String toRfidAccess = toSaveBookingForRoom + "/rfid/" + cardParser();

    FirebaseJson rfidContent;
    rfidContent.set("fields/owner/stringValue", cardOwner);

    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toRfidAccess.c_str(), rfidContent.raw(), "")) {
    } else {
    draw("something went wrong :(");
    delay(5000);
    }

  
  } else if(quickBookType == 2){
   

    int currentTime = (formattedTime.substring(0, 2) + formattedTime.substring(3, 6)).toInt();
    int nextMeetingTime = (nextMeeting.substring(0, 2) + nextMeeting.substring(3, 6)).toInt();
    int timeDiff = nextMeetingTime - currentTime;
    String startTime = "";
    String endTime = "";

    
        
       
    if(formattedTime.substring(3, 6).toInt() <= 30){
      startTime = formattedTime.substring(0, 2) + ":00";
      } else {
      startTime = formattedTime.substring(0, 2) + ":30";
      }
      
      if(timeDiff < 100 && nextMeeting != nullptr){
        endTime = nextMeeting;
        } else {
        endTime = String(startTime.substring(0,2).toInt() + 1) + ":" + startTime.substring(3,6);
          
        }
         FirebaseJson content;
       
        content.set("fields/startTime/stringValue", startTime);
        content.set("fields/endTime/stringValue", endTime);
        content.set("fields/owner/stringValue", cardOwner);
        content.set("fields/title/stringValue", "quick booking");
       
        
        
        String toSaveBookingForRoom = "test/" + uid + "/" + currentDate + "/" + startTime;

         if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toSaveBookingForRoom.c_str(), content.raw(), "")) {
    } else {
      draw("something went wrong :(");
      delay(5000);
    }

    String toSaveBookingForUser = "users/" + cardOwner + "/" + currentDate + "/" + startTime;
    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toSaveBookingForUser.c_str(), content.raw(), "")) {
    } else {
      draw("something went wrong :(");
      delay(5000);
    }

    String toRfidAccess = toSaveBookingForRoom + "/rfid/" + cardParser();

    FirebaseJson rfidContent;
    rfidContent.set("fields/owner/stringValue", cardOwner);

    if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", toRfidAccess.c_str(), rfidContent.raw(), "")) {
    } else {
      draw("something went wrong :(");
      delay(5000);
    }

  }
  
  
  quickBookType = 0;
  cardOwner = "";
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