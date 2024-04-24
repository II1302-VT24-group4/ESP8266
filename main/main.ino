/*
 * This code uses the U8g2 library for monochrome displays.
 * The library is licensed under the GPL v3 license.
 * More details: https://github.com/olikraus/u8g2/wiki
 */

/****************************************************
 *               Includes               
 *****************************************************/

#include "pitches.h"
#include "ESP8266WiFi.h"
#include "U8g2lib.h"
#include "SoftwareSerial.h"

/****************************************************
 *               WiFI parameters               
 *****************************************************/

const char* ssid = "Christoffers iPhone 12";
const char* password = "test123";

//const char* ssid = "KTH-IoT";
//const char* password = "LRVsNdJ8bAkHWt6lACzW";

/****************************************************
 *               Pin parameters               
 *****************************************************/

#define RES D1       // /RES (Reset signal) connected to D2 (GPIO4)
#define CS D8        // /CS (Chip Select Signal) connected to D8 (GPIO15)
#define RS D1        // RS (Register Select) signal connected to D1 (GPIO5)
#define SC D5        // SCL (serial mode) signal connected to D5 (GPIO14)
#define SI D7        // SDI (serial mode) signal connected to D7 (GPIO13)
#define LED_RED D0   // (GPIO 0)
#define LED_GREEN 10 // (GPIO 2)
#define BUZZER D6    // (GPIO 12)
#define BUTTON_CONFIRM 3 // (GPIO3)
#define BUTTON_ABORT 1 // (GPIO1)
#define BUTTONS A0 // A0 (ADC)
#define RX 4
#define TX 0

/****************************************************
 *               Global variabels           
 *****************************************************/

// Initialize the display using hardware SPI
U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ CS, /* dc=*/ RS, /* reset=*/ RES);

// RFID
SoftwareSerial SoftSerial(RX, TX);
unsigned char buffer[64];       // buffer array for data receive over serial port
int count = 0;                    // counter for buffer array
String cardNumber = "";

/****************************************************
 *           Initialization For controller           
 *****************************************************/

void setup()
{
  SoftSerial.begin(9600);     // the SoftSerial baud rate
  Serial.begin(9600);         // the Serial port of Arduino baud rate.

  // Screen
  u8g2.begin();  // Initialize the display
  u8g2.setContrast(100); // Adjust the contrast level (0-255)

  // LEDS
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);
  pinMode(BUZZER, INPUT_PULLUP);

  // WiFI
  //connectToWiFi(ssid, password);

  // Buttons
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_ABORT, INPUT_PULLUP);
}

/*****************************************************
 *           Loop Function, to run repeatedly         
 *****************************************************/

void loop()
{
  if (SoftSerial.available())              
  {
      while(SoftSerial.available())               // reading data into char array
      {
          buffer[count++] = SoftSerial.read();      // writing data into array
          if(count == 64)break;
      }
      Serial.write(buffer, count);     // if no data transmission ends, write buffer to hardware serial port
      cardNumber = String((char*)buffer);
      clearBufferArray();             // call clearBufferArray function to clear the stored data from the array
      count = 0;                      // set counter of while loop to zero
      tone(BUZZER, 440);
      u8g2.firstPage();  // Start a page to write graphics
      do {
        delay(50);
        draw(cardNumber.c_str());  // Call the draw function where the graphics commands are executed
      } while ( u8g2.nextPage() );  // Continue to the next page
      delay(600);
  }
  if (Serial.available())             // if data is available on hardware serial port ==> data is coming from PC or notebook
  SoftSerial.write(Serial.read());    // write it to the SoftSerial shield

  byte button_confirm_state = digitalRead(BUTTON_CONFIRM);
  byte button_abort_state = digitalRead(BUTTON_ABORT);
  int buttons_direction = analogRead(BUTTONS); // For moving left, right, up and down.
  
  u8g2.firstPage();  // Start a page to write graphics
  do {
    draw("Room 420");  // Call the draw function where the graphics commands are executed
  } while ( u8g2.nextPage() );  // Continue to the next page

  if (button_confirm_state == LOW || button_abort_state == LOW) {
      Serial.println("Button is pressed");
      tone(BUZZER, 540);

      if(button_confirm_state == LOW){
          digitalWrite(LED_GREEN, HIGH); 
          u8g2.firstPage();  // Start a page to write graphics
          do {
            draw("Confirm");  // Call the draw function where the graphics commands are executed
          } while ( u8g2.nextPage() );  // Continue to the next page
          delay(100);
      } else{
          digitalWrite(LED_RED, HIGH); 
          u8g2.firstPage();  // Start a page to write graphics
          do {
            draw("Abort");  // Call the draw function where the graphics commands are executed
          } while ( u8g2.nextPage() );  // Continue to the next page
          delay(100);
      }
  }
  else if(button_confirm_state == HIGH && button_abort_state == HIGH && (buttons_direction < 8)){
      Serial.println("Button is not pressed");
      noTone(BUZZER);
      digitalWrite(LED_RED, LOW); 
      digitalWrite(LED_GREEN, LOW); 
  }
  else if(buttons_direction < 225 && buttons_direction > 210){
    u8g2.firstPage();
    do{
      draw("Left");
    } while(u8g2.nextPage());
    delay(100);
  }
  else if(buttons_direction < 60 && buttons_direction > 50){
    u8g2.firstPage();
    do{
      draw("Right");
    } while(u8g2.nextPage());
    delay(100);
  }
  else if(buttons_direction < 117 && buttons_direction > 110){
    u8g2.firstPage();
    do{
      draw("Up");
    } while(u8g2.nextPage());
    delay(100);
  }
  else if(buttons_direction < 910 && buttons_direction > 900){
    u8g2.firstPage();
    do{
      draw("Down");
    } while(u8g2.nextPage());
    delay(100);
  }
  else{
    
  }
}