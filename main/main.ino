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
#include "Firebase_ESP_Client.h"
#include <addons/TokenHelper.h>

/****************************************************
 *               WiFI & Firebase parameters          
 *****************************************************/

const char* ssid = "iPhone";
const char* password = "89korvkorv";

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
 *               Pin parameters               
 *****************************************************/

#define RES D1       // /RES (Reset signal) connected to D2 (GPIO4)
#define CS D8        // /CS (Chip Select Signal) connected to D8 (GPIO15)
#define RS D1        // RS (Register Select) signal connected to D1 (GPIO5)
#define SC D5        // SCL (serial mode) signal connected to D5 (GPIO14)
#define SI D7        // SDI (serial mode) signal connected to D7 (GPIO13)
#define LED_RED D3   // (GPIO 9)
#define LED_GREEN D4 // (GPIO 10)
#define BUZZER D6    // (GPIO 12)
#define BUTTON_CONFIRM 3 // (GPIO 3 RX)
#define BUTTON_ABORT 1 // (GPIO 1 TX)
#define BUTTONS A0 // A0 (ADC)

// Initialize the display using hardware SPI
U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ CS, /* dc=*/ RS, /* reset=*/ RES);

/****************************************************
 *           Initialization For controller           
 *****************************************************/

void setup()
{
  Serial.begin(115200);

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
  //Get Document
  //--------------------
  path = "test/" + uid;

  Serial.print("Get a document... ");
  delay(1000);
  if (Firebase.Firestore.getDocument(&fbdo, PROJECT_ID, "", path.c_str(), "")) {

    
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    delay(1000);
    

    

    // Create a FirebaseJson object and set content with received payload
    FirebaseJson payload;
    payload.setJsonData(fbdo.payload().c_str());
    delay(1000);



    // Get the data from FirebaseJson object 
    FirebaseJsonData jsonData;
    payload.get(jsonData, "fields/test/stringValue", true);

    String test = jsonData.stringValue;

    Serial.println(jsonData.stringValue);
    delay(1000);

    u8g2.firstPage();
    do{
      draw(test.c_str());
    } while(u8g2.nextPage());
  }
 
  /*u8g2.firstPage();
    do{
      draw(FIREBASE_CLIENT_VERSION);
    } while(u8g2.nextPage());*/

  /*u8g2.firstPage();
  do{
    draw(uid.c_str());
  } while(u8g2.nextPage());*/
    
  delay(100);
}