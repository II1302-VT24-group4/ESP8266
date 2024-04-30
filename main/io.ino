/**
 * @file io.ino
 * @brief Hardware io code
 * @details
 */

void uppdateButtons() {
  button_confirm_state = digitalRead(BUTTON_CONFIRM);
  button_abort_state = digitalRead(BUTTON_ABORT);
  buttons_direction = analogRead(BUTTONS); // For moving left, right, up and down.
}

String getButtonState() {
  if (button_confirm_state == LOW) {
    return "Confirm";
  } else if (button_abort_state == LOW) {
    return "Abort";
  } else if(buttons_direction < 225 && buttons_direction > 210){
    return "Left";
  } else if(buttons_direction < 60 && buttons_direction > 50){
    return "Right";
  } else if(buttons_direction < 118 && buttons_direction > 109){
    return "Up";
  } else if(buttons_direction < 911 && buttons_direction > 899){
    return "Down";
  } else {
    return "No button pressed";
  }
}