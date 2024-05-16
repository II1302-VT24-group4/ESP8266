/**
 * @file io.ino
 * @author Christoffer Franzén
 * @brief Manages hardware I/O operations.
 */

/**
 * @brief Reads button and ADC pin states.
 * 
 * Updates global variables with the state of confirm and abort buttons, and the ADC pin value.
 */
void uppdateButtons() {
  button_confirm_state = digitalRead(BUTTON_CONFIRM);
  button_abort_state = digitalRead(BUTTON_ABORT);
  buttons_direction = analogRead(BUTTONS);
}

/**
 * @brief Returns the state of the buttons.
 * 
 * Checks each button's state and returns a string representing the pressed button.
 * 
 * @return String indicating which button is pressed.
 */
String getButtonState() {
  if (button_confirm_state == LOW) return "Confirm";
  if (button_abort_state == LOW) return "Abort";
  if (buttons_direction < 880 && buttons_direction > 870) return "Left";
  if (buttons_direction < 46 && buttons_direction > 37) return "Right";
  if (buttons_direction < 189 && buttons_direction > 180) return "Up";
  if (buttons_direction < 93 && buttons_direction > 84) return "Down";
  return "No button pressed";
}