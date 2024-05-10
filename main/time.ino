/**
 * @file time.ino
 * @author Christoffer Franzén, Erik Heiskanen, Leo Andersson
 * @brief Code for managing time functionalities.
 */

/**
 * @brief Updates the current time and date.
 *
 * @details This code is taken from https://randomnerdtutorials.com/
 * 
 * Retrieves the current time from a time client and updates global variables.
 */
void updateTime() {
  timeClient.update();

  formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);

  time_t epochTime = timeClient.getEpochTime();

  struct tm *ptm = gmtime((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;

  currentDate =
    String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
}

/**
 * @brief Checks if the current time is within a specified range.
 * 
 * Checks if the current time is between two given times.
 * 
 * @param currentTime Current time formatted as "HH:MM".
 * @param startTime Start time of the range formatted as "HH:MM".
 * @param endTime End time of the range formatted as "HH:MM".
 * @return bool True if current time is within the range, false otherwise.
 */
bool timeInRange(String currentTime, String startTime, String endTime) {
  int start = (startTime.substring(0, 2) + startTime.substring(3, 6)).toInt();
  int end = (endTime.substring(0, 2) + endTime.substring(3, 6)).toInt();
  int current = (currentTime.substring(0, 2) + currentTime.substring(3, 6)).toInt();

  if (start <= current && current < end) {
    currentMeetingID = startTime.substring(0, 5);
    roomAvailable = false;
    return false;
  }
  currentMeetingID = "";
  roomAvailable = true;
  return true;
}

/**
 * @brief Finds the next available time slot.
 * 
 * Searches for the next available time slot based on the current meeting ID,
 * start times, and end times.
 * 
 * @param startTime Array of start times formatted as "HH:MM".
 * @param endTime Array of end times formatted as "HH:MM".
 * @return String Time range of the next available slot.
 */
String nextFreeSlot(String *startTime, String *endTime) {
  String result = "";
  int i = 0;

  while ((currentMeetingID.substring(0, 2) + currentMeetingID.substring(3, 6)).toInt() != (startTime[i].substring(0, 2) + startTime[i].substring(3, 6)).toInt()) {
    i++;
  }
  Serial.println(startTime[i + 1]);
  int j = i;

  while ((endTime[j].substring(0, 2) + endTime[j].substring(3, 6)).toInt() == (startTime[j + 1].substring(0, 2) + startTime[j + 1].substring(3, 6)).toInt()) {

    if ((endTime[j].substring(0, 2) + endTime[j].substring(3, 6)).toInt() != (startTime[j + 1].substring(0, 2) + startTime[j + 1].substring(3, 6)).toInt()) {
      result = endTime[j] + " - " + startTime[j + 1];
    }
    j++;
  }

  if (startTime[j + 1] == "") {
    result = "from: " + endTime[j];
  }

  if ((endTime[j].substring(0, 2) + endTime[j].substring(3, 6)).toInt() != (startTime[j + 1].substring(0, 2) + startTime[j + 1].substring(3, 6)).toInt()) {
    result = endTime[j] + " - " + startTime[j + 1];
  }

  return result;
}