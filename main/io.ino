/**
 * @file io.ino
 * @brief Hardware io code
 * @details 
 */

/**
 * @brief Updates the state of the buttons.
 * 
 * This function reads the state of the confirm and abort buttons, and the direction of the directional buttons.
 * The state of each button is stored in a global variable.
 */
void uppdateButtons() {
  button_confirm_state = digitalRead(BUTTON_CONFIRM);
  button_abort_state = digitalRead(BUTTON_ABORT);
  buttons_direction = analogRead(BUTTONS); // For moving left, right, up and down.
}

/**
 * @brief Gets the state of the buttons.
 * 
 * This function checks the state of each button and returns a string representing the state.
 * If a button is pressed, the function returns the name of the button.
 * 
 * @return String representing the state of the buttons.
 */
String getButtonState() {
  if (button_confirm_state == LOW) {
    return "Confirm";
  } else if (button_abort_state == LOW) {
    return "Abort";
  } else if(buttons_direction < 225 && buttons_direction > 210){
    return "Left";
  } else if(buttons_direction < 70 && buttons_direction > 40){
    return "Right";
  } else if(buttons_direction < 128 && buttons_direction > 100){
    return "Up";
  } else if(buttons_direction < 921 && buttons_direction > 889){
    return "Down";
  } else {
    return "No button pressed";
  }
}