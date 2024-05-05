/**
 * @file main.ino
 * @brief The main code for the project
 * @details GPL v3 license.
 */

/****************************************************
 *               Test
 *****************************************************/

#define RUN_TEST_PROGRAM
//#undef RUN_TEST_PROGRAM // Uncomment this line if you want to run the test program

#define DEBUG_ON 1
#define DEBUG_OFF 0
byte debugMode = DEBUG_OFF;

#define DBG(...) debugMode == DEBUG_ON ? Serial.println(__VA_ARGS__) : NULL

/****************************************************
 *               Includes
 *****************************************************/

#include "ESP8266WiFi.h"         // Finns defualt
#include "Firebase_ESP_Client.h" // Library manager Firebase_ESP_Client
#include "NTPClient.h"           // Library manager NTPClient
#include "SoftwareSerial.h"      // Finns defualt
#include "U8g2lib.h"             // Library manager U8g2 (This code uses the U8g2 library for monochrome displays.)
#include "WiFiUdp.h"             // Finns defualt
#include "addons/TokenHelper.h"  // Finns om man laddar ner Firebase_ESP_Client
#include "pitches.h"             // Sketch -> Include Libary -> Add .ZIP Libary (https://github.com/hibit-dev/buzzer/tree/master/lib)
#include "ArduinoJson.h" // finns i library manager

/****************************************************
 *               WiFI parameters
 *****************************************************/

const char *const ssids[] = {"Christoffers iPhone 12", "iPhone", "KTH-IoT"};
const char *const passwords[] = {"89korvkorv", "89korvkorv", "LRVsNdJ8bAkHWt6lACzW"};

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

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData dbData;
String uid;
String path;
String firebaseStatus = "on";

// Initialize the display using hardware SPI
U8G2_ST7565_NHD_C12864_2_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/CS, /* dc=*/RS,
                                        /* reset=*/RES);

// RFID
SoftwareSerial SoftSerial(RX, TX);
unsigned char buffer[64]; // buffer array for data receive over serial port
int count = 0;            // counter for buffer array
String cardNumber = "";

// States
enum State
{
  IDLE,
  BOOK,
  RECEIVE_RFID_DATA,
  DISPLAY_CARD
};

State currentState = IDLE;
bool cardPresent = false;                  // Flag to check the presence of the card
unsigned long lastRFIDReadTime = 0;        // Timestamp of the last RFID read
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

// global stuffs and potential war crimes
String currentMeetingID = "";
bool roomAvailable = false;
const int MAX_DOCUMENTS = 10; // Max antal dokument du förväntar dig hantera
const int TIME_LENGTH = 6;
String startTimes[48];
String endTimes[48];
int documentsCount = 0;
String nextAvailableTimeSlot = "";
String nextAvailableTime = "";

// Calender
unsigned long lastCalendarUpdateTime = 0;
const unsigned long calendarUpdateInterval = 500; // Interval in milliseconds (0.5 seconds)

unsigned long lastButtonUpdateTime = 0;
const unsigned long buttonUpdateInterval = 50; // Update buttons every 50 milliseconds


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

void loop() {
#ifdef RUN_TEST_PROGRAM
  test();
#else
  stateMachine();
#endif
}