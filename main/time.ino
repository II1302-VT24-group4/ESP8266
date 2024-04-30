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
