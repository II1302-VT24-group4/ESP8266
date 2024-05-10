/**
 * @file draw.ino
 * @author Christoffer Franzén, Hein Lee
 * @brief Display code.
 * @details This file contains functions for drawing on the display.
 */

static unsigned char lock_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xFF, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0xF0, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 
  0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1F, 0xF0, 0x3F, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0x01, 0x80, 0xFF, 0x00, 0x00, 0x00, 0x80, 0x7F, 0x00, 
  0x00, 0xFE, 0x01, 0x00, 0x00, 0x80, 0x1F, 0x00, 0x00, 0xFC, 0x01, 0x00, 
  0x00, 0xC0, 0x1F, 0x00, 0x00, 0xF0, 0x03, 0x00, 0x00, 0xE0, 0x07, 0x00, 
  0x00, 0xF0, 0x07, 0x00, 0x00, 0xF0, 0x03, 0x00, 0x00, 0xE0, 0x0F, 0x00, 
  0x00, 0xF0, 0x03, 0x00, 0x00, 0xC0, 0x0F, 0x00, 0x00, 0xF8, 0x01, 0x00, 
  0x00, 0x80, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x80, 0x1F, 0x00, 
  0x00, 0xFC, 0x00, 0x00, 0x00, 0x07, 0x3F, 0x00, 0x00, 0xFC, 0x00, 0x00, 
  0x80, 0x0F, 0x3F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x80, 0x1F, 0x3E, 0x00, 
  0x00, 0x7C, 0x00, 0x00, 0xE0, 0x0F, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0x00, 
  0xF0, 0x07, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0x00, 0xF8, 0x03, 0x3E, 0x00, 
  0x00, 0x7E, 0x00, 0x01, 0xFC, 0x01, 0x7E, 0x00, 0x00, 0x3E, 0x80, 0x03, 
  0xFE, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0xC0, 0x07, 0x7F, 0x00, 0x7E, 0x00, 
  0x00, 0x7C, 0xC0, 0x8F, 0x3F, 0x00, 0x3E, 0x00, 0x00, 0x7C, 0x80, 0xDF, 
  0x1F, 0x00, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0xFF, 0x0F, 0x00, 0x3E, 0x00, 
  0x00, 0x7C, 0x00, 0xFE, 0x07, 0x00, 0x3E, 0x00, 0x00, 0xFC, 0x00, 0xFC, 
  0x03, 0x00, 0x3F, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x01, 0x00, 0x1F, 0x00, 
  0x00, 0xF8, 0x00, 0xF0, 0x00, 0x00, 0x1F, 0x00, 0x00, 0xF8, 0x01, 0x60, 
  0x00, 0x80, 0x1F, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x80, 0x0F, 0x00, 
  0x00, 0xF0, 0x03, 0x00, 0x00, 0xC0, 0x0F, 0x00, 0x00, 0xE0, 0x07, 0x00, 
  0x00, 0xE0, 0x07, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0xF0, 0x07, 0x00, 
  0x00, 0xC0, 0x1F, 0x00, 0x00, 0xF8, 0x03, 0x00, 0x00, 0x80, 0x7F, 0x00, 
  0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x80, 0xFF, 0x00, 0x00, 
  0x00, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 
  0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0xE0, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 
  0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/**
 * @brief Draws an unlocked lock icon on the display.
 * 
 */
void drawUnlockedLockIcon() {
  u8g2.drawXBM((128 - lock_width) / 2, (64 - lock_height) / 2, lock_width, lock_height, lock_bits);
}

/**
 * @brief Draws the booking screen.
 * 
 * @param str The string to display.
 */
void draw(const char *str) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, str);
  } while (u8g2.nextPage());
}

/**
 * @brief Draws the idle state.
 * 
 * This function is responsible for drawing the idle state of the display.
 */
void drawIdle() {
  u8g2.setFont(u8g2_font_ncenB14_tr);

  // Create an array of strings to display

  String text3 = "";
  String text4 = "";

  if (nextAvailableTime.isEmpty() && nextMeeting.isEmpty()) {
    text3 = "Available";
    text4 = "";
  } else if(!nextAvailableTime.isEmpty()){
    text3 = "Booked until";
    text4 = nextAvailableTime;
  } else if(!nextMeeting.isEmpty()){
    text3 = "Next booking";
    text4 = "-" + nextMeeting;
  }

  String displayTexts[] = { "Anna", formattedTime.substring(0, formattedTime.length() - 3), text3, text4 };

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    // Iterate over the displayTexts array and print each string centered on the display
    for (int i = 0; i < 4; i++) {
      int textWidth = u8g2.getStrWidth(displayTexts[i].c_str());
      u8g2.setCursor((128 - textWidth) / 2, 15 + i * 15);
      u8g2.print(displayTexts[i].c_str());
    }

    //u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(24, 31, 9200);
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
 * @brief 
 * 
 */

void drawConfirmBooking(String timeText) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x12_t_symbols);
    u8g2.drawStr(0, 13, "Do you want to book:");
    u8g2.drawStr(0, 28, timeText.c_str());
    u8g2.drawStr(0, 43, "Click Confirm");
    u8g2.drawStr(0, 58, "or Abort");
  } while (u8g2.nextPage());
}