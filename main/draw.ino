/**
 * @file draw.ino
 * @brief Display code
 * @details This code is by Christoffer Franzén
 */

/**
 * @brief Draws the booking screen
 * 
 */
void draw(const char *str) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, str);
  } while (u8g2.nextPage());
}

/**
 * @brief Draws the idle state
 * 
 * This function is responsible for drawing the idle state of the display.
 */
void drawIdle() {
  u8g2.setFont(u8g2_font_ncenB14_tr);

  // Create an array of strings to display
  String displayTexts[] = { "Anna", formattedTime.substring(0, formattedTime.length() - 3), "Next time", nextAvailableTime.isEmpty() ? "Free" : nextAvailableTime };

  u8g2.firstPage();
  do {
    // Iterate over the displayTexts array and print each string centered on the display
    for (int i = 0; i < 4; i++) {
      int textWidth = u8g2.getStrWidth(displayTexts[i].c_str());
      u8g2.setCursor((128 - textWidth) / 2, 15 + i * 15);
      u8g2.print(displayTexts[i].c_str());
    }
  } while (u8g2.nextPage());

  if (nextAvailableTime.isEmpty()) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  } else {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }
}

/**
 * @brief Draws the booking screen
 * 
 */
void drawDefaultCalender() {
  String displayText = currentDate + " | " + formattedTime;

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 10);
    u8g2.print(displayText.c_str());
    u8g2.drawBox(0, 11, 128, 1);
    u8g2.setCursor(0, 22);
    u8g2.print("Current: Vacant");
    u8g2.setCursor(0, 32);

    if (cursor == 0) {
      u8g2.print("08:00 Free <-");
    } else {
      u8g2.print("08:00 Free");
    }

    u8g2.setCursor(0, 42);

    if (cursor == 1) {
      u8g2.print("09:00 Free <-");
    } else {
      u8g2.print("09:00 Free");
    }

    u8g2.setCursor(0, 52);

    if (cursor == 2) {
      u8g2.print("10:00 Free <-");
    } else {
      u8g2.print("10:00 Free");
    }

    u8g2.setCursor(0, 62);
    u8g2.print(getButtonState().c_str());
  } while (u8g2.nextPage());
}