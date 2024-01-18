// max30100
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"


// Initialize the pulse oximeter
PulseOximeter pox;
void onBeatDetected()
{
  Serial.println("Heartbeat detected!");
}


// DS18B20 temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//float RawHigh = 201.2;
//float RawLow = 35.15;
//float RawLow = 40.15;
//float ReferenceHigh = 212;       // temperature of boil water = 100 celcius/212 fahrenheit
//float ReferenceLow = 32;         // temperature of ice = 0 celcius/32 fahrenheit
//float RawRange = RawHigh - RawLow;
//float ReferenceRange = ReferenceHigh - ReferenceLow;



//firebase
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
//Provide the token generation process info.
#include <addons/TokenHelper.h>
//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
/* 1. Define the WiFi credentials */
//#define WIFI_SSID "Ane ki"
//#define WIFI_PASSWORD "0987654321"
//#define WIFI_SSID "Afridi"
//#define WIFI_PASSWORD "Af0191144"
//#define WIFI_SSID "iPhone 6s plus"
//#define WIFI_PASSWORD "Afr0191144"
#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "11345678"
// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino
/* 2. Define the API Key */
#define API_KEY "LnvVgW68PINAoaLy7MsRlwC3iRiKwpd1Jy5y2WHp"
/* 3. Define the RTDB URL */
#define DATABASE_URL "https://cse499-cd2ac-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


// SSD1306 display
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// multitasking delays
int task1Delay = 100;    // 0.1 seconds delay/100 milisecond delay
int task2Delay = 10000; // 10 seconds delay/10000 milisecond delay


// golobal variable
int bpm;
int SpO2;
float tempF;

void setup() {
  // protocol begin
  Serial.begin(9600);
  Wire.begin();
  sensors.begin();

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    delay(2000);
    Serial.println("");
    ESP.restart();
  }
  // Clear the buffer
  display.clearDisplay();

  // Set the text color to white
  display.setTextSize(2);/////////accepts an integer number as a size. The greater the number, the bigger the text would be. Smallest size is 1 which is the default size //of texts.
  display.setTextColor(WHITE);///////////////specify the color using display.setTextColor() WHITE/BLACK
  display.setCursor(15, 8);//////////////////specify where on the display we're going to display the text.
  display.println(F("CSE-499"));////////////We draw something on by calling on our (display) object,
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.println(F("Al Mehedi Hasan"));
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.println(F("MD Maruf Chowdhury"));
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println(F("Istiak Ahmed Sheam"));
  display.display();//////////////////Call the display.display() function to make the actual drawing happen on the hardware level.

  // Wifi connection Setup
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("Wifi Connected");
  Serial.println();


  /* Assign the api key (required) */
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.setDoubleDigits(5);
  if (!Firebase.ready()) {
    Serial.println("Firebase Connection fail !");
    Serial.println("");
  }

  // Initialize the pulse oximeter
  pox.begin();
  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);

  if (!pox.begin()) {
    Serial.println("Oximeter initialization Fail !");
    delay(2000);
    Serial.println("");
    ESP.restart();
  }
}

void loop() {
  static unsigned long lastTask1RunTime = 0;
  static unsigned long lastTask2RunTime = 0;
  //static unsigned long lastTask3RunTime = 0;

  unsigned long currentMillis = millis();

  if (currentMillis - lastTask1RunTime >= task1Delay) {
    lastTask1RunTime = currentMillis;
    task1();
  }
  if (currentMillis - lastTask2RunTime >= task2Delay) {
    lastTask2RunTime = currentMillis;
    task2();
  }


}

void task1() {
  pox.update(); // Update the pulse oximeter readings

  // Print the heart rate and oxygen saturation data
  if (pox.getHeartRate() > 20 && pox.getSpO2() > 50 && pox.getSpO2() < 160) {
    bpm   = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    Serial.print("Heart rate: ");
    Serial.print(bpm);
    Serial.print(" bpm   ");
    Serial.print("SpO2: ");
    Serial.print(SpO2);
    Serial.println("%");
  }
}

void task2() {
  pox.shutdown();

  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  //tempF = (tempC * 1.8) + 32;
  //float CorrectedValue = (((tempF - RawLow) * ReferenceRange) / RawRange) + ReferenceLow;
  //float CorrectedValue = (((tempC - RawLow) * ReferenceRange) / RawRange) + ReferenceLow;
  delay(100); // 0.1 seconds delay/100 milisecond delay
  //delay(200); // 0.2 seconds delay/200 milisecond delay
  display.clearDisplay();

  display.setCursor(5, 5);
  display.setTextSize(2);
  display.println(F("T: "));
  display.setCursor(35, 5);
  display.println((tempC));
  //display.println((CorrectedValue));
  display.drawCircle(105, 5, 2, WHITE);
  display.setCursor(110, 5);
  display.println(F("C"));

if(bpm == 0 && SpO2 == 0){
  display.setCursor(5, 35);
  display.setTextSize(1);
  display.println(F("BPM and SPO2 are     being measured."));
}


else{
  display.setCursor(5, 25);
  display.setTextSize(2);
  display.println(F("BPM : "));
  display.setCursor(65, 25);
  display.println((bpm));

  
  display.setCursor(5, 45);
  display.println(F("SpO2: "));
  display.setCursor(65, 45);
  display.println((SpO2));
  display.setCursor(105, 45);
  display.println(F("%"));
}

  // Display the contents of the buffer (i.e. show the text on the screen)
  display.display();

  Serial.println("");
  Serial.println("");
  Serial.println("--- Sending Final Value --- ");
  Serial.print("Heart rate: "); Serial.print(bpm); Serial.println(" bpm");
  Serial.print("SpO2: "); Serial.print(SpO2); Serial.println("%");
  Serial.print("Temperature: "); Serial.print(tempC); Serial.println(" °C");
  //Serial.print("Temperature: "); Serial.print(CorrectedValue); Serial.println(" °C");
  Serial.println("");
  Serial.println("");
  
  //Send data to firebase
  if (bpm != 0 && SpO2 != 0){
  Firebase.setInt(fbdo, "/Data/BPM", bpm);
  Firebase.setInt(fbdo, "/Data/SP02", SpO2);
  }
  Firebase.setFloat(fbdo, "/Data/Temperature", tempC);
  //Firebase.setFloat(fbdo, "/Data/Temperature", CorrectedValue);

  bpm = 0;
  SpO2 = 0;

  pox.resume();
}
