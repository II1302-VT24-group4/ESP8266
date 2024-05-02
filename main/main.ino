/**
 * @file main.ino
 * @brief The main code for the project
 * @details GPL v3 license.
 */

/****************************************************
 *               Includes
 *****************************************************/

#include "ESP8266WiFi.h"         // Finns defualt
#include "Firebase_ESP_Client.h" // Library manager Firebase_ESP_Client
#include "NTPClient.h"           // Library manager NTPClient
#include "SoftwareSerial.h"      // Finns defualt
#include "U8g2lib.h" // Library manager U8g2 (This code uses the U8g2 library for monochrome displays.)
#include "WiFiUdp.h" // Finns defualt
#include "addons/TokenHelper.h" // Finns om man laddar ner Firebase_ESP_Client
#include "pitches.h" // Sketch -> Include Libary -> Add .ZIP Libary (https://github.com/hibit-dev/buzzer/tree/master/lib)
#include <Arduino.h> // Finns defualt
#include <SPI.h>     // Finns defualt


/****************************************************
 *               WiFI parameters
 *****************************************************/

const char *ssids[] = {"Christoffers iPhone 12", "iPhone", "KTH-IoT"};
const char *passwords[] = {"89korvkorv", "89korvkorv", "LRVsNdJ8bAkHWt6lACzW"};

/****************************************************
 *               Firebase parameters
 *****************************************************/

#define API_KEY "AIzaSyB1vrEG7FRHz0qx1usl56Nph130NA-KUMM"
#define PROJECT_ID "meeting-planner-56526"
#define FIREBASE_AUTH "vc2VyiUJEQZyZrck7bkNDX932JG3"

#define USER_EMAIL "test@test.com"
#define USER_PASSWORD "test123"

// Define Firebase Data object
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
#define BUTTON_ABORT 1   // TX
#define BUTTONS A0
#define RX D2 // D2
#define TX D3 // D3

/****************************************************
 *               Global variabels
 *****************************************************/

// Initialize the display using hardware SPI
// U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ CS, /* dc=*/ RS, /*
// reset=*/ RES); U8G2_ST7565_NHD_C12864_1_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ CS,
// /* dc=*/ RS, /* reset=*/ RES);
U8G2_ST7565_NHD_C12864_2_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/CS, /* dc=*/RS,
                                        /* reset=*/RES);

// RFID
SoftwareSerial SoftSerial(RX, TX);
unsigned char buffer[64]; // buffer array for data receive over serial port
int count = 0;            // counter for buffer array
String cardNumber = "";

// States
enum State { IDLE, BOOK, RECEIVE_RFID_DATA, DISPLAY_CARD };

State currentState = IDLE;
bool cardPresent = false;           // Flag to check the presence of the card
unsigned long lastRFIDReadTime = 0; // Timestamp of the last RFID read
const unsigned long removalTimeout = 1000; // Timeout in milliseconds (1 second)

// Time
String formattedTime;
String currentDate;

// Buttons
byte button_confirm_state;
byte button_abort_state;
int buttons_direction;

// Cursor
int cursor = 0;

unsigned long debounceDuration = 150; // millis
unsigned long lastTimeButtonStateChanged = 0;

/****************************************************
 *           Initialization For controller
 *****************************************************/

void setup() {
  SoftSerial.begin(9600); // the SoftSerial baud rate
  Serial.begin(9600);     // the Serial port of Arduino baud rate.

  // Screen
  u8g2.begin();          // Initialize the display
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
  delay(500);
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(500);
  }

  // Print uid to console
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  delay(500);
  Serial.println(uid);
  delay(500);

  // Buttons
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_ABORT, INPUT_PULLUP);
}

/*****************************************************
 *           Loop Function, to run repeatedly
 *****************************************************/

void loop() {
  updateTime();
  uppdateButtons();
  String displayText = currentDate + " | " + formattedTime;

  switch (currentState) {
  case IDLE:
    drawIdle(formattedTime);
    
    // Om användaren trycker på någon knapp
    if(button_confirm_state == LOW || button_abort_state == LOW){
      currentState = BOOK;
    }
  break;

  case BOOK:
    drawDefaultCalender(displayText);

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