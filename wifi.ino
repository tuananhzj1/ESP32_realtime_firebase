#include <BluetoothSerial.h>
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


/* 1. Define the API Key */
#define API_KEY "AIzaSyD0Mdjl19RiOp2QKd90d8-h436IhUQpDhg"

/* 2. Define the RTDB URL */
#define DATABASE_URL "https://smarthome-92669-default-rtdb.firebaseio.com/" 

/* 3. Define the user Email and password that alreadey registerd or added in
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

// GPIO Pin 13
const int GPIO_Pin13 = 13;
const int GPIO_Pin18 = 18;

// Login or not
String isLogin = "/esp32/Login";
int statusLogin = 0;

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
BluetoothSerial SerialBT;

String ssid = "";
String password = "";

void setup()
{
  pinMode(GPIO_Pin13, OUTPUT); // status firebase connection
  pinMode(GPIO_Pin21, OUTPUT);
  pinMode(GPIO_Pin22, OUTPUT);
  pinMode(GPIO_Pin23, OUTPUT);
  pinMode(GPIO_Pin18, OUTPUT);
  digitalWrite(GPIO_Pin18, HIGH); // Power check
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT");
  // Set the Bluetooth device name
  Serial.println("Waiting for Bluetooth connection...");
  // Wait for a Bluetooth connection
  while (!SerialBT.connected()) {
    delay(100);
  }

  while (SerialBT.available() == 0) {
    String input = SerialBT.readString();
    input.trim(); // Remove leading/trailing whitespaces
    
    if (input.startsWith("SSID=")) {
      ssid = input.substring(5); 
      ssid.trim();
      // Serial.println("Received SSID: " + ssid);
      SerialBT.println(F("Recived SSID!"));
    } else if (input.startsWith("PASSWORD=")) {
      password = input.substring(9);
      password.trim();
      // Serial.println("Received password: " + password);
      SerialBT.println(F("Recived password!"));
    } else if (input.startsWith("SCAN")) {
      Serial.println(F("Scanning wifi visible"));
      SerialBT.print(F("=====================================\n"));
      int n = WiFi.scanNetworks();
      if (n > 0) {
        for (int i = 0; i < n; i++) {
          SerialBT.print(WiFi.SSID(i));
          SerialBT.print('\n');
          delay(0);
        }
      }
      SerialBT.print(F("============Final scanning===========\n"));
    } else if (input.startsWith("CONNECT")) {
      SerialBT.end(); // Must close bluetooth connection before moving next step
      break;
    } else if (input.startsWith("DEFAULT")) {
      ssid = "TuanAnh";
      password = "12345Abc@";
      SerialBT.end(); // Must close bluetooth connection before moving next step
      break;
    }
  }

  // Convert string to const char* for input 
  const char* wifi_ssid = ssid.c_str();
  const char* wifi_password = password.c_str();

  // Begin connect to wifi
  WiFi.begin(wifi_ssid, wifi_password);
  Serial.print("Connecting to Wi-Fi");
  int count = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    count++;
    if (count > 20) {
      Serial.println(F("FAILED Connection to wifi!"));
    }
  }
  Serial.print(F("\nConnected with IP: "));
  Serial.println(WiFi.localIP());

  // Prepare config for connection to Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  // Connect to Firebase
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  // Check status login or not of user
  Firebase.RTDB.getInt(&fbdo, isLogin, &statusLogin);
  if (statusLogin) {
    digitalWrite(GPIO_Pin13, HIGH); // connect successfull
    digitalWrite(GPIO_Pin18, LOW); // power check 
    // GPIO Pin23
    Firebase.RTDB.getInt(&fbdo, inputGPIO_Pin23, &statusGPIO_23);
    if (statusGPIO_23 == 1)
    {
      digitalWrite(GPIO_Pin23, LOW);
      Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin23, statusGPIO_23);
      Serial.println("Light 23 OFF");
    }
    else
    {
      digitalWrite(GPIO_Pin23, HIGH);
      Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin23, statusGPIO_23);
      Serial.println("Light 23 ON");
    }

    // GPIO Pin22
    Firebase.RTDB.getInt(&fbdo, inputGPIO_Pin22, &statusGPIO_22);
    if (statusGPIO_22 == 1)
    {
      digitalWrite(GPIO_Pin22, LOW);
      Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin22, statusGPIO_22);
      Serial.println("Light 22 OFF");
    }
    else
    {
      digitalWrite(GPIO_Pin22, HIGH);
      Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin22, statusGPIO_22);
      Serial.println("Light 22 ON");
    }

    // GPIO Pin21
    Firebase.RTDB.getInt(&fbdo, inputGPIO_Pin21, &statusGPIO_21);
    if (statusGPIO_21 == 1)
    {
      digitalWrite(GPIO_Pin21, LOW);
      Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin21, statusGPIO_21);
      Serial.println("Light 21 OFF");
    }
    else
    {
      digitalWrite(GPIO_Pin21, HIGH);
      Firebase.RTDB.setIntAsync(&fbdo, outputGPIO_Pin21, statusGPIO_21);
      Serial.println("Light 21 ON");
    }
    Serial.println("===================================================");
  }
  // delay(5000);
}
