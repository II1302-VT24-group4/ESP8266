/**
 * @file draw.ino
 * @brief Display code
 * @details This code is by Christoffer Franzén
 */

void draw(const char *str) {
  u8g2.setFont(u8g2_font_ncenB08_tr); // Set the font for drawing text
  u8g2.drawStr(0, 10, str); // Draw the input string at position (0, 10)
}

/**
 * @brief Draws the idle state
 * 
 */
void drawIdle() {
  u8g2.firstPage();
  do {
    // Set the font to a larger size
    u8g2.setFont(u8g2_font_ncenB14_tr);

    String roomName = "Anna";
    int roomNameWidth = u8g2.getStrWidth(roomName.c_str());
    u8g2.setCursor((128 - roomNameWidth) / 2, 15);
    u8g2.print(roomName.c_str());

    String time = formattedTime;
    int timeWidth = u8g2.getStrWidth(time.c_str());
    u8g2.setCursor((128 - timeWidth) / 2, 30);
    u8g2.print(time.c_str());

    String nextText = "Nästa tid";
    int nextTextWidth = u8g2.getStrWidth(nextText.c_str());
    u8g2.setCursor((128 - nextTextWidth) / 2, 45);
    u8g2.print(nextText.c_str());

    String nextSlot = nextAvailableTime;
    int nextSlotWidth = u8g2.getStrWidth(nextSlot.c_str());
    u8g2.setCursor((128 - nextSlotWidth) / 2, 60);
    u8g2.print(nextSlot.c_str());

  } while (u8g2.nextPage());
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
    u8g2.print("Nuvarande: Ledigt");
    u8g2.setCursor(0, 32);

    if (cursor == 0) {
      u8g2.print("08:00 Ledigt <-");
    } else {
      u8g2.print("08:00 Ledigt");
    }

    u8g2.setCursor(0, 42);

    if (cursor == 1) {
      u8g2.print("09:00 Ledigt <-");
    } else {
      u8g2.print("09:00 Ledigt");
    }

    u8g2.setCursor(0, 52);

    if (cursor == 2) {
      u8g2.print("10:00 Ledigt <-");
    } else {
      u8g2.print("10:00 Ledigt");
    }

    u8g2.setCursor(0, 62);
    u8g2.print(getButtonState().c_str());
  } while (u8g2.nextPage());
}