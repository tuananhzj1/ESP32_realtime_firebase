#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "TuanAnh"
#define WIFI_PASSWORD "12345Abc@"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyD0Mdjl19RiOp2QKd90d8-h436IhUQpDhg"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://smarthome-92669-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or \
                           //<databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in
 * your project */
#define USER_EMAIL "tuananhnguyen@gmail.com"
#define USER_PASSWORD "TuanAnhNguyen"

// GPIO Pin 23
String inputGPIO_Pin23 = "/esp32/GPIO23/input";
String outputGPIO_Pin23 = "/esp32/GPIO23/output";
int statusGPIO_23 = 0;
const int GPIO_Pin23 = 23;

// GPIO Pin 22
String inputGPIO_Pin22 = "/esp32/GPIO22/input";
String outputGPIO_Pin22 = "/esp32/GPIO22/output";
int statusGPIO_22 = 0;
const int GPIO_Pin22 = 22;

// GPIO Pin 21
String inputGPIO_Pin21 = "/esp32/GPIO21/input";
String outputGPIO_Pin21 = "/esp32/GPIO21/output";
int statusGPIO_21 = 0;
const int GPIO_Pin21 = 21;

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void setup()
{
  pinMode(GPIO_Pin21, OUTPUT);
  pinMode(GPIO_Pin22, OUTPUT);
  pinMode(GPIO_Pin23, OUTPUT);
  Serial.begin(115200);

  #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
  #else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  #endif
    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(300);
  #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
      if (millis() - ms > 10000)
        break;
  #endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
  #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    config.wifi.clearAP();
    config.wifi.addAP(WIFI_SSID, WIFI_PASSWORD);
  #endif

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);
  #if defined(ESP8266)
    fbdo.setBSSLBufferSize(512, 2048);
  #endif
}

void loop()
{
  // GPIO Pin23
  Firebase.RTDB.getInt(&fbdo, inputGPIO_Pin23, &statusGPIO_23);
  if (statusGPIO_23 == 1)
  {
    digitalWrite(GPIO_Pin23, LOW);
    Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin23, statusGPIO_23);
    Serial.println("Light 23 ON");
  }
  else
  {
    digitalWrite(GPIO_Pin23, HIGH);
    Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin23, statusGPIO_23);
    Serial.println("Light 23 OFF");
  }

  // GPIO Pin22
  Firebase.RTDB.getInt(&fbdo, inputGPIO_Pin22, &statusGPIO_22);
  if (statusGPIO_22 == 1)
  {
    digitalWrite(GPIO_Pin22, LOW);
    Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin22, statusGPIO_22);
    Serial.println("Light 22 ON");
  }
  else
  {
    digitalWrite(GPIO_Pin22, HIGH);
    Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin22, statusGPIO_22);
    Serial.println("Light 22 OFF");
  }

  // GPIO Pin21
  Firebase.RTDB.getInt(&fbdo, inputGPIO_Pin21, &statusGPIO_21);
  if (statusGPIO_21 == 1)
  {
    digitalWrite(GPIO_Pin21, LOW);
    Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin21, statusGPIO_21);
    Serial.println("Light 21 ON");
  }
  else
  {
    digitalWrite(GPIO_Pin21, HIGH);
    Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin21, statusGPIO_21);
    Serial.println("Light 21 OFF");
  }
  Serial.println("===================================================");

}
