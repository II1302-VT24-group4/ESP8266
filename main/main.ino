/**
 * @file main.ino
 * @brief The main code for the project
 * @details GPL v3 license.
 */

/****************************************************
 *               Test
 *****************************************************/

#define RUN_TEST_PROGRAM
#undef RUN_TEST_PROGRAM // Uncomment this line if you want to run the test program

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
#include "Arduino.h"             // Finns defualt
#include "SPI.h"                 // Finns defualt
#include "ArduinoJson.h" // finns i library manager

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

unsigned long debounceDuration = 150; // millis
unsigned long lastTimeButtonStateChanged = 0;

int val = 0;


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


void updateDailyCalendar(){
  String pathToMeetings = "test/" + uid + "/" + currentDate;
  Serial.println(pathToMeetings);
  
  if(Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", pathToMeetings.c_str(), "")){
    // Create a FirebaseJson object and set content with received payload
    Serial.println("Före payload");
    FirebaseJson payload;
    Serial.println("Efter payload");
    payload.setJsonData(fbdo.payload().c_str());
    Serial.println(fbdo.payload().c_str());
    Serial.println("Efter SetJson");
    delay(100);
    
    String jsonString = fbdo.payload().c_str();
    bool available = parseJson(jsonString, formattedTime);
    Serial.println(available);
    
    if(available){
    String path2 = "test/" + uid;

    // Create document to send to firebase
    FirebaseJson content;
    content.set("fields/available/booleanValue", true);

      if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", path2.c_str(), content.raw(), "available")) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          roomAvailable = true;

          
      } else {
          Serial.println(fbdo.errorReason());
      }
    } else {
      String path2 = "test/" + uid;

      // Create document to send to firebase
      FirebaseJson content;
      content.set("fields/available/booleanValue", false);

      if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", path2.c_str(), content.raw(), "available")) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          roomAvailable = false;

          
      } else {
          Serial.println(fbdo.errorReason());
      }

    }

  } else {
    String path2 = "test/" + uid;

    // Create document to send to firebase
    FirebaseJson content;
    content.set("fields/available/booleanValue", true);

      if (Firebase.Firestore.patchDocument(&fbdo, PROJECT_ID, "", path2.c_str(), content.raw(), "available")) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());

          
      } else {
        Serial.println(fbdo.errorReason());
      }
    roomAvailable = true;
    currentMeetingID = "";  
  }
}

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

void loop()
{
#ifdef RUN_TEST_PROGRAM
  test();
#else
  stateMachine();
#endif
}