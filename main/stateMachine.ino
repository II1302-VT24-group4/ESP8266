/**
 * @file stateMachine.ino
 * @brief
 * @details
 */

void stateMachine(){
  updateTime();
  uppdateButtons();

  switch (currentState) {
  case IDLE:
    drawIdle();
    
    // Om användaren trycker på någon knapp
    if(button_confirm_state == LOW || button_abort_state == LOW){
      currentState = BOOK;
    }
  break;

  case BOOK:
    drawDefaultCalender();

    // Om användaren trycker abort gå tillbacka till start state
    if(getButtonState() == "Abort"){
      currentState = IDLE;
      delay(150);
    }

    if(getButtonState() == "Down"){
      cursor++;

      if(cursor >= 3){
        cursor = 0;
      }

      delay(150);
    }

    if(getButtonState() == "Up"){
      cursor--;

      if(cursor < 0){
        cursor = 2;
      }

      delay(150);
    }

    // Om rfid blip
    if (SoftSerial.available()) {
      currentState = RECEIVE_RFID_DATA;
    }
  break;

  case RECEIVE_RFID_DATA:
    if (readRFIDData()) {
      currentState = DISPLAY_CARD;
      cardPresent = true; // Set card presence flag
    }
    break;

  case DISPLAY_CARD:
    displayRFIDData();
    if (!SoftSerial.available() && cardPresent) {
      if (detectCardRemoval()) {
        cardPresent = false; // Card is removed
      }
    } else if (!cardPresent && SoftSerial.available()) {
      // Detect fresh card presentation
      if (readRFIDData()) { // Make sure it's a valid card re-presentation
        currentState =
            IDLE; // Transition back to IDLE on second card presentation
      }
    }
    break;
  }
}