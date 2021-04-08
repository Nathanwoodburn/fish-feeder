///////////////Set these settings//////////////////////////////
const char* ssid = "SSID";  //Wifi SSID (Name)
const char* password = "PASSWORD"; //Wifi Password
unsigned long myTalkBackID = 00000; //Talkback ID
const char * myTalkBackKey = "TALKBACKKEY"; //Talkback Key
///////////////////////////////////////////////////////////////

//Permenent Stuff DO NOT CHANGE!!!
const char* host = "api.thingspeak.com";
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
WiFiClient client;

////////////STEPPER MOTOR PINS////////////////////////
#define A 15
#define B 13
#define C 12
#define D 14

#define NUMBER_OF_STEPS_PER_REV 520 // Number of steps to perform each feed



void setup() {
  Serial.begin(9600); // Start serial connection
  Serial.println("Starting"); //Print to serial
  pinMode(A, OUTPUT); //Set pin modes
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  feedfish(); //Feed fish - Code Below
}

void loop() {
  // Connect or reconnect to Wi-Fi
  if (WiFi.status() != WL_CONNECTED) { // If wifi isn't connected
    Serial.print("Connecting to WIFI "); 
    
    WiFi.hostname("Fish Feeder");
    while (WiFi.status() != WL_CONNECTED) { //While wifi isn't connected
      WiFi.begin(ssid, password); //connect to wifi
      Serial.print(".");
      delay(1000); //wait for wifi to connect
    }
    Serial.println("\nConnected");
  }

  // Create the TalkBack URI
  String tbURI = String("/talkbacks/") + String(myTalkBackID) + String("/commands/execute");

  // Create the message body for the POST out of the values
  String postMessage =  String("api_key=") + String(myTalkBackKey);

  // Make a string for any commands that might be in the queue
  String newCommand = String();

  // Make the POST to ThingSpeak
  int x = httpPOST(tbURI, postMessage, newCommand);
  client.stop();

  // Check the result
  if (x == 200) {
    if (newCommand.length() != 0) {
      if (newCommand.indexOf("Feed") >= 0) {

        feedfish();
      }
      else
      {
        Serial.println("Feed command not found");
      }
    }
    else {
      Serial.println("Feed command not found");
    }

  }
  else {
    Serial.println("Problem checking queue. HTTP error code " + String(x)); //Error with wifi connection
  }
  delay(60000); // Wait 60 seconds to check queue again
}
void feedfish() { //Feed Fish Code
  Serial.println("Feeding Fish"); //Print to serial
  int i; // Create a variable to store current step
  i = 0; 
  while (i < NUMBER_OF_STEPS_PER_REV) { //While step number is less than steps needed
    onestep(); // Step motor
    i++; //increase i by 1
  }

}
void writemt(int a, int b, int c, int d) { // Control stepper outputs
  digitalWrite(A, a);
  digitalWrite(B, b);
  digitalWrite(C, c);
  digitalWrite(D, d);
}
void onestep() { // Step motor
  writemt(1, 0, 0, 0);
  delay(5);
  writemt(1, 1, 0, 0);
  delay(5);
  writemt(0, 1, 0, 0);
  delay(5);
  writemt(0, 1, 1, 0);
  delay(5);
  writemt(0, 0, 1, 0);
  delay(5);
  writemt(0, 0, 1, 1);
  delay(5);
  writemt(0, 0, 0, 1);
  delay(5);
  writemt(1, 0, 0, 1);
  delay(5);
}
int httpPOST(String uri, String postMessage, String &response) { //Get HTTP connection

  bool connectSuccess = false;
  connectSuccess = client.connect("api.thingspeak.com", 80);

  if (!connectSuccess) {
    return -301;
  }

  postMessage += "&headers=false";

  String Headers =  String("POST ") + uri + String(" HTTP/1.1\r\n") +
                    String("Host: api.thingspeak.com\r\n") +
                    String("Content-Type: application/x-www-form-urlencoded\r\n") +
                    String("Connection: close\r\n") +
                    String("Content-Length: ") + String(postMessage.length()) +
                    String("\r\n\r\n");

  client.print(Headers);
  client.print(postMessage);

  long startWaitForResponseAt = millis();
  while (client.available() == 0 && millis() - startWaitForResponseAt < 5000) {
    delay(100);
  }

  if (client.available() == 0) {
    return -304; // Didn't get server response in time
  }

  if (!client.find(const_cast<char *>("HTTP/1.1"))) {
    return -303; // Couldn't parse response (didn't find HTTP/1.1)
  }

  int status = client.parseInt();
  if (status != 200) {
    return status;
  }

  if (!client.find(const_cast<char *>("\n\r\n"))) {
    return -303;
  }

  String tempString = String(client.readString());
  response = tempString;

  return status;
}
