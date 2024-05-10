/**
 * @file main.ino
 * @author Christoffer Franzén, Erik Heiskanen, Leo Andersson, Hein Lee
 * @brief Main code for the project.
 * @details This file contains the main functionality of the project.
 * GPL v3 license.
 */

/****************************************************
 *               Test
 *****************************************************/

#define RUN_TEST_PROGRAM
#undef RUN_TEST_PROGRAM  // Uncomment this line if you want to run the test program

/****************************************************
 *               Includes
 *****************************************************/

#include "ESP8266WiFi.h"          // Finns defualt
#include "Firebase_ESP_Client.h"  // Library manager Firebase_ESP_Client
#include "NTPClient.h"            // Library manager NTPClient
#include "SoftwareSerial.h"       // Finns defualt
#include "U8g2lib.h"              // Library manager U8g2 (This code uses the U8g2 library for monochrome displays.)
#include "WiFiUdp.h"              // Finns defualt
#include "addons/TokenHelper.h"   // Finns om man laddar ner Firebase_ESP_Client
#include "pitches.h"              // Sketch -> Include Libary -> Add .ZIP Libary (https://github.com/hibit-dev/buzzer/tree/master/lib)
#include "Arduino.h"              // Finns defualt
#include "SPI.h"                  // Finns defualt
#include "ArduinoJson.h"          // finns i library manager

/****************************************************
 *               WiFI parameters
 *****************************************************/

const char *ssids[] = { "KTH-IoT", "Christoffers iPhone 12", "iPhone" };
const char *passwords[] = { "LRVsNdJ8bAkHWt6lACzW", "89korvkorv", "89korvkorv" };

/****************************************************
 *               Firebase parameters
 *****************************************************/

#define API_KEY "AIzaSyB1vrEG7FRHz0qx1usl56Nph130NA-KUMM"
#define PROJECT_ID "meeting-planner-56526"
#define FIREBASE_AUTH "vc2VyiUJEQZyZrck7bkNDX932JG3"
#define USER_EMAIL "test@test.com"
#define USER_PASSWORD "test123"

/****************************************************
 *               NTP parameters
 *****************************************************/

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
#define LED_GREEN 10  // SD3
#define BUZZER D6
#define BUTTON_CONFIRM 3  // RX
#define BUTTON_ABORT 1    // TX
#define BUTTONS A0
#define RX D3  // D2
#define TX D2  // D3

/****************************************************
 *               Global parameters
 *****************************************************/

// Bitmap stuff
#define lock_width 64
#define lock_height 64

#define TONE_ABORT 330
#define TONE_CONFIRM 550
#define TONE_RFID 660

/****************************************************
 *               Global variabels
 *****************************************************/

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData dbData;
String uid;
String path;
String firebaseStatus = "on";

// Display
U8G2_ST7565_NHD_C12864_2_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/CS, /* dc=*/RS,
                                        /* reset=*/RES);

// RFID
SoftwareSerial SoftSerial(RX, TX);
unsigned char buffer[64];
int count = 0;
String cardNumber = "";
String cardOwner = "";

// States
enum State {
  IDLE,
  QUICKBOOK,
  CONFIRMQUICKBOOK,
  NEXTROOM
};

State currentState = IDLE;
unsigned long lastRFIDReadTime = 0;

// Time
String formattedTime;
String currentDate;

// Buttons
byte button_confirm_state;
byte button_abort_state;
int buttons_direction;

// Cursor
int cursor = 0;

// Global stuff
String currentMeetingID = "";
bool roomAvailable = false;
const int MAX_DOCUMENTS = 10;
const int TIME_LENGTH = 6;
String startTimes[48];
String endTimes[48];
int documentsCount = 0;
String nextAvailableTimeSlot = "";
String nextAvailableTime = "";
int quickBookType = 0;
String nextMeeting = "";

unsigned long lastButtonUpdateTime = 0;
const unsigned long buttonUpdateInterval = 200;

unsigned long lastCalendarUpdateTime = 0;
const unsigned long calendarUpdateInterval = 2000;

/****************************************************
 *           Initialization For controller
 *****************************************************/

void setup() {
#ifdef RUN_TEST_PROGRAM
  setupTest();
#else
  setupStateMachine();
#endif
}

/*****************************************************
 *           Loop Function, to run repeatedly
 *****************************************************/

void loop() {
#ifdef RUN_TEST_PROGRAM
  test();
#else
  stateMachine();
#endif
}