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

#include <Arduino.h>
#include <SPI.h>
#include "pitches.h"              // Sketch -> Include Libary -> Add .ZIP Libary (https://github.com/hibit-dev/buzzer/tree/master/lib)
#include "ESP8266WiFi.h"          // Finns defualt
#include "U8g2lib.h"              // Library manager U8g2
#include "SoftwareSerial.h"       // Finns defualt
#include "Firebase_ESP_Client.h"  // Library manager Firebase_ESP_Client
#include "addons/TokenHelper.h"   // Finns om man laddar ner Firebase_ESP_Client
#include "NTPClient.h"            // Library manager NTPClient
#include "WiFiUdp.h"              // Finns defualt

/****************************************************
 *               WiFI parameters          
 *****************************************************/

const char* ssids[] = {"Christoffers iPhone 12", "iPhone", "KTH-IoT"};
const char* passwords[] = {"89korvkorv", "89korvkorv", "LRVsNdJ8bAkHWt6lACzW"};

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
//U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ CS, /* dc=*/ RS, /* reset=*/ RES);
//U8G2_ST7565_NHD_C12864_1_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ CS, /* dc=*/ RS, /* reset=*/ RES);
U8G2_ST7565_NHD_C12864_2_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ CS, /* dc=*/ RS, /* reset=*/ RES);

// RFID
SoftwareSerial SoftSerial(RX, TX);
unsigned char buffer[64];       // buffer array for data receive over serial port
int count = 0;                    // counter for buffer array
String cardNumber = "";

uint8_t cursor = 0;

// State machine
enum State {
  IDLE,
  RECEIVE_RFID_DATA,
  DISPLAY_CARD
};


State currentState = IDLE;
bool cardPresent = false; // Flag to check the presence of the card
String formattedTime;
String currentDate;

unsigned long lastRFIDReadTime = 0; // Timestamp of the last RFID read
const unsigned long removalTimeout = 1000; // Timeout in milliseconds (1 second)

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
  for (int i = 0; i < sizeof(ssids) / sizeof(ssids[0]); i++) {
    connectToWiFi(ssids[i], passwords[i]);
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
  }

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(7200);

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
  updateTime();
  String displayText = currentDate + " | " + formattedTime;

  // Buttons
  byte button_confirm_state = digitalRead(BUTTON_CONFIRM);
  byte button_abort_state = digitalRead(BUTTON_ABORT);
  int buttons_direction = analogRead(BUTTONS); // For moving left, right, up and down.

  switch (currentState) {
    case IDLE:
      drawDefaultCalender(displayText);

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
      if (readRFIDData()) {  // Make sure it's a valid card re-presentation
        currentState = IDLE;  // Transition back to IDLE on second card presentation
        }
      }
    break;
  }
}