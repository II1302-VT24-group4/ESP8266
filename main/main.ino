/*
 * This code uses the U8g2 library for monochrome displays.
 * The library is licensed under the GPL v3 license.
 * More details: https://github.com/olikraus/u8g2/wiki
 */

 /* 
 * The FirebaseJsonData object holds the returned data which can be read from the following properties.
 * jsonData.stringValue - contains the returned string.
 * jsonData.intValue - contains the returned integer value.
 * jsonData.floatValue - contains the returned float value.
 * jsonData.doubleValue - contains the returned double value.
 * jsonData.boolValue - contains the returned boolean value.
 * jsonData.success - used to determine the result of the get operation.
 * jsonData.type - used to determine the type of returned value in string represent 
 * the types of value e.g. string, int, double, boolean, array, object, null and undefined.
*/

/****************************************************
 *               Includes               
 *****************************************************/

#include "pitches.h"
#include "ESP8266WiFi.h"
#include "U8g2lib.h"
#include "SoftwareSerial.h"
#include "Firebase_ESP_Client.h"
#include "addons/TokenHelper.h"
#include "NTPClient.h"
#include "WiFiUdp.h"

/****************************************************
 *               WiFI parameters          
 *****************************************************/

const char* ssid = "iPhone";
const char* password = "89korvkorv";

//const char* ssid = "KTH-IoT";
//const char* password = "LRVsNdJ8bAkHWt6lACzW";

/****************************************************
 *               Firebase parameters          
 *****************************************************/

#define API_KEY "AIzaSyB1vrEG7FRHz0qx1usl56Nph130NA-KUMM"
#define PROJECT_ID "meeting-planner-56526"
#define FIREBASE_AUTH "vc2VyiUJEQZyZrck7bkNDX932JG3"

#define USER_EMAIL "test@test.com"
#define USER_PASSWORD "test123"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData dbData;
String uid;
String path;
String firebaseStatus = "on";

/****************************************************
 *               NTP parameters               
 *****************************************************/

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

/****************************************************
 *               Pin parameters               
 *****************************************************/

#define RES D4
#define CS D8
#define RS D1
#define SC D5   
#define SI D7
#define LED_RED D0
#define LED_GREEN 10 // SD3
#define BUZZER D6
#define BUTTON_CONFIRM 3 // RX
#define BUTTON_ABORT 1 // TX
#define BUTTONS A0
#define RX D2 // D2
#define TX D3 // D3

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

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(7200);

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
  connectToWiFi(ssid, password);

  // Set up config API KEY
  config.api_key = API_KEY;

  // Set up firebase auth
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Generate token
  config.token_status_callback = tokenStatusCallback;

  // Connect to firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Get userID
  Serial.println("Getting User UID");
  delay(1000);
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }

  // Print uid to console
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  delay(1000);
  Serial.println(uid);
  delay(1000);
           
  // Buttons
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_ABORT, INPUT_PULLUP);
}

/*****************************************************
 *           Loop Function, to run repeatedly         
 *****************************************************/

void loop()
{
  // Time stuff
  timeClient.update();

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime); 

  time_t epochTime = timeClient.getEpochTime();

  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);

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
      String displayText = formattedTime + " Date: " + currentDate;
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
  
  String displayText = formattedTime + " Date: " + currentDate;

  u8g2.firstPage();
  do{
    draw(displayText.c_str());
  } while(u8g2.nextPage());

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