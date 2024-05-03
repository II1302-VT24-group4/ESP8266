/**
 * @file time.ino
 * @brief Time code
 * @details This code is taken from https://randomnerdtutorials.com/ and
 * modified by Christoffer Franzén
 */

/**
 * @brief Updates the current time and date.
 *
 * This function retrieves the current time from a time client, formats it,
 * and updates the global variables `formattedTime` and `currentDate`.
 */
void updateTime() {
  // Update the time client
  timeClient.update();

  // Get and print the formatted time
  formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);

  // Get the epoch time
  time_t epochTime = timeClient.getEpochTime();

  // Convert the epoch time to a tm struct
  struct tm *ptm = gmtime((time_t *)&epochTime);

  // Extract the day, month, and year
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;

  // Construct the current date string
  currentDate =
      String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
}

/*
  @function: timeInRange will check if time is in between two diffrent times, it checks down to the minute. But NOT seconds! Returns true if the current time is between
  @function: two diffrent times. Or false if the current time is outside of the range.
  @params: String currentTime, String startTime, String endTime
  @return: bool
*/

bool timeInRange(String currentTime, String startTime, String endTime){
  
  int start = (startTime.substring(0, 2) + startTime.substring(3, 6)).toInt();
  int end = (endTime.substring(0, 2) + endTime.substring(3, 6)).toInt();
  int current = (currentTime.substring(0, 2) + currentTime.substring(3, 6)).toInt();
  Serial.println(current);
  Serial.println(start);
  Serial.println(end);
  
  if(start <= current && current < end){
    Serial.println("occupadööööö");
    currentMeetingID = startTime.substring(0, 5);
    roomAvailable = false;
    return false;
  }
  Serial.println("aviablööö");
  currentMeetingID = "";
  roomAvailable = true;
  return true;
}

String nextFreeSlot(String *startTime, String *endTime){
  String result = "";
  int i = 0;
  Serial.println((currentMeetingID.substring(0, 2) + currentMeetingID.substring(3, 6)).toInt());
  Serial.println((startTime[i].substring(0, 2) + startTime[i].substring(3, 6)).toInt());

  while((currentMeetingID.substring(0, 2) + currentMeetingID.substring(3, 6)).toInt() != (startTime[i].substring(0, 2) + startTime[i].substring(3, 6)).toInt()){
    Serial.println("I while loopen!");
    i++;
  }
  Serial.println(startTime[i+1]);
  int j = i;
 
  while((endTime[j].substring(0, 2) + endTime[j].substring(3, 6)).toInt() == (startTime[j+1].substring(0, 2) + startTime[j+1].substring(3, 6)).toInt()){
    //Serial.println("thockis");
   
    if((endTime[j].substring(0, 2) + endTime[j].substring(3, 6)).toInt() != (startTime[j+1].substring(0, 2) + startTime[j+1].substring(3, 6)).toInt()){
      result = endTime[j] + " - " + startTime[j+1];
    }
    j++;
  }

   if(startTime[j+1] == ""){
      result = "from: " + endTime[j];
  }

  if((endTime[j].substring(0, 2) + endTime[j].substring(3, 6)).toInt() != (startTime[j+1].substring(0, 2) + startTime[j+1].substring(3, 6)).toInt()){
      result = endTime[j] + " - " + startTime[j+1];
  }
  //Serial.println("korv" + result);
  return result;
}