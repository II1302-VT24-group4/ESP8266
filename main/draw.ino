/*
 * This code is used to display text 
 */

 

void draw(const char* str) {
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Set the font for drawing text
  u8g2.drawStr(0, 10, str);  // Draw the input string at position (0, 10)
}

void drawDefaultCalender(String date) {

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 10);
    u8g2.print(date.c_str());
    u8g2.drawBox(0,11,128,1);
    u8g2.setCursor(0, 22);
    u8g2.print("Nuvarande: Ledigt");
    u8g2.setCursor(0, 32);
    u8g2.print("08:00 Ledigt <-");
    u8g2.setCursor(0, 42);
    u8g2.print("09:00 Bokat");
    u8g2.setCursor(0, 52);
    u8g2.print("10:00 Ledigt");
    u8g2.setCursor(0, 62);
    u8g2.print("11:00 Ledigt");
  } while (u8g2.nextPage());
}