/**********************************************************************************
 *  TITLE: Blynk 2.0 + Manual Switch (Latched) control 4 Relays using NodeMCU (Real time feedback) (No Wi-Fi control)
 *  I have Modified the Blynk.Edgent.Edgent_ESP8266 example code (https://blynk.io/) for this project
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/O9VYZqWPNEQ
 *  Related Blog : https://iotcircuithub.com/home-automation-using-nodemcu-and-blynk/
 *  by Subhajit (Tech StudyCell)*/

// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLx8nAPtIe"
#define BLYNK_DEVICE_NAME "Home Automation"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"
 *  https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Preferences--> Aditional boards Manager URLs : 

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
 *  Blynk 1.0.1 Library:  https://github.com/blynkkk/blynk-library
 **********************************************************************************/
 *  Download Board ESP8266 NodeMCU : https://github.com/esp8266/Arduino
 *  Download the libraries

//#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD


// define the GPIO connected with Relays and switches
#define RelayPin1 5  //D1
#define RelayPin2 4  //D2
#define RelayPin3 14 //D5
#define RelayPin4 12 //D6

#define SwitchPin1 10  //SD3
#define SwitchPin2 D3   //D3 
#define SwitchPin3 13  //D7
#define SwitchPin4 3   //RX

#define wifiLed   16   //D0

//Change the virtual pins according the rooms
#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4

// Relay State
bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; //Define integer to remember the toggle state for relay 2
bool toggleState_3 = LOW; //Define integer to remember the toggle state for relay 3
bool toggleState_4 = LOW; //Define integer to remember the toggle state for relay 4

// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;

#include "BlynkEdgent.h"
#include "DHT.h"
#define DHTPIN 01 
#define DHTTYPE DHT11
#define buzzer 9

int mq2 = A0; // smoke sensor is connected with the analog pin A0 
int data = 0; 

DHT dht(DHTPIN, DHTTYPE);
float t, h;

void sendSensor()
{
  h = dht.readHumidity();
  t = dht.readTemperature();  
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void getSendData()
{
data = analogRead(mq2); 
  Blynk.virtualWrite(V7, data);
 
  if (data >= 600 )
  {
    Blynk.notify("Smoke Detected!"); 
    digitalWrite(buzzer, HIGH);
    
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
 
}
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  if(toggleState_1 == 1){
    digitalWrite(RelayPin1, LOW);
  }
  else { 
    digitalWrite(RelayPin1, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  if(toggleState_2 == 1){
    digitalWrite(RelayPin2, LOW);
  }
  else { 
    digitalWrite(RelayPin2, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  if(toggleState_3 == 1){
    digitalWrite(RelayPin3, LOW);
  }
  else { 
    digitalWrite(RelayPin3, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  if(toggleState_4 == 1){
    digitalWrite(RelayPin4, LOW);
  }
  else { 
    digitalWrite(RelayPin4, HIGH);
  }
}


void setup()
{
  Serial.begin(115200);
  dht.begin();
  delay(100);
  
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);

  digitalWrite(wifiLed, HIGH);

  BlynkEdgent.begin();

  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
  Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
  Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
  Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
  edgentTimer.setInterval(1000L, sendSensor);
  edgentTimer.setInterval(1000L, getSendData);

}

void loop() 
{

    BlynkEdgent.run();
    edgentTimer.run(); // Initiates SimpleTimer
    
    manual_control(); //Manual Switch Control   
}
