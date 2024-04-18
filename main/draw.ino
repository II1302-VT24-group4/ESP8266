void draw(const char* str) {
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Set the font for drawing text
  u8g2.drawStr(0, 10, str);  // Draw the input string at position (0, 10)
}
