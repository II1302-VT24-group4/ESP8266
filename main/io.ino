/**
 * @file io.ino
 * @brief Hardware io code
 * @details
 */

void uppdateButtons() {
  byte button_confirm_state = digitalRead(BUTTON_CONFIRM);
  byte button_abort_state = digitalRead(BUTTON_ABORT);
  int buttons_direction =
      analogRead(BUTTONS); // For moving left, right, up and down.
}