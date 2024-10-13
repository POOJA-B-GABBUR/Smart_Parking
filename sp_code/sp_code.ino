// Include necessary libraries
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

// Blynk authentication token
char auth[] = "KbiYL4u8UfxNGy3BFtfFKA3GrxYohEc2";

// WiFi credentials
char ssid[] = "Pooja";
char pass[] = "pooja9880";

SimpleTimer timer; // Timer object for scheduling events

String myString; // Complete message from Arduino, consisting of sensor data
char rdata; // Received character

int firstVal, secondVal, thirdVal; // Sensor values
int led1, led2; // LED states

// Timer event function to send Arduino's uptime to Virtual Pin (1)
void myTimerEvent()
{
  // Send uptime in seconds
  Blynk.virtualWrite(V1, millis() / 1000);
}

void setup()
{
  Serial.begin(9600); // Initialize serial communication

  Blynk.begin(auth, ssid, pass); // Connect to Blynk using authentication token and WiFi credentials

  // Set timer intervals for sensor value functions
  timer.setInterval(1000L, sensorvalue1);
  timer.setInterval(1000L, sensorvalue2);
}

void loop()
{
  // If no data available from serial communication
  if (Serial.available() == 0)
  {
    Blynk.run(); // Run Blynk
    timer.run(); // Run timer events
  }

  // If data available from serial communication
  if (Serial.available() > 0)
  {
    rdata = Serial.read(); // Read character from serial

    myString = myString + rdata; // Append character to complete message string

    // If end of message reached
    if (rdata == '\n')
    {
      // Process the received message
      String l = getValue(myString, ',', 0); // Extract first value from message
      String m = getValue(myString, ',', 1); // Extract second value from message

      led1 = l.toInt(); // Convert first value to integer and store in led1 variable
      led2 = m.toInt(); // Convert second value to integer and store in led2 variable

      myString = ""; // Clear the message string for the next message
    }
  }
}

// Function to send sensor value 1 to Blynk
void sensorvalue1()
{
  int sdata = led1;
  Blynk.virtualWrite(V9, sdata);
}

// Function to send sensor value 2 to Blynk
void sensorvalue2()
{
  int sdata = led2;
  Blynk.virtualWrite(V11, sdata);
}

// Function to extract a specific value from a string
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  // Iterate over the characters in the string
  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    // If the separator character is found or end of string reached
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++; // Increment the found counter
      strIndex[0] = strIndex[1] + 1; // Set the start index for the next value
      strIndex[1] = (i == maxIndex) ? i + 1 : i; // Set the end index for the current value
    }
  }

  // Return the extracted value as a substring
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

#include <SoftwareSerial.h>

SoftwareSerial nodemcu(2, 3); // SoftwareSerial object for communication with NodeMCU

int parking1_slot1_ir_s = 4; // Pin number for parking slot 1 infrared sensor
int parking1_slot2_ir_s = 5; // Pin number for parking slot 2 infrared sensor

String sensor1; // String to store sensor 1 value
String sensor2; // String to store sensor 2 value

String cdata = ""; // Complete data consisting of sensor values

void setup()
{
  Serial.begin(9600); // Initialize serial communication with Arduino
  nodemcu.begin(9600); // Initialize serial communication with NodeMCU

  pinMode(parking1_slot1_ir_s, INPUT); // Set parking slot 1 infrared sensor pin as input
  pinMode(parking1_slot2_ir_s, INPUT); // Set parking slot 2 infrared sensor pin as input
}

void loop()
{
  p1slot1(); // Check status of parking slot 1
  p1slot2(); // Check status of parking slot 2

  cdata = cdata + sensor1 + "," + sensor2 + ","; // Concatenate sensor values with comma delimiter
  Serial.println(cdata); // Print the complete data to Arduino serial monitor
  nodemcu.println(cdata); // Send the complete data to NodeMCU

  delay(6000); // Delay for 6 seconds
  cdata = ""; // Clear the complete data string

  digitalWrite(parking1_slot1_ir_s, HIGH); // Set parking slot 1 infrared sensor pin to HIGH
  digitalWrite(parking1_slot2_ir_s, HIGH); // Set parking slot 2 infrared sensor pin to HIGH
}

void p1slot1() // Function to check status of parking slot 1
{
  if (digitalRead(parking1_slot1_ir_s) == LOW)
  {
    sensor1 = "255"; // If infrared sensor is triggered, set sensor1 value to "255"
    delay(200); // Delay to debounce the sensor
  }
  if (digitalRead(parking1_slot1_ir_s) == HIGH)
  {
    sensor1 = "0"; // If infrared sensor is not triggered, set sensor1 value to "0"
    delay(200); // Delay to debounce the sensor
  }
}

void p1slot2() // Function to check status of parking slot 2
{
  if (digitalRead(parking1_slot2_ir_s) == LOW)
  {
    sensor2 = "255"; // If infrared sensor is triggered, set sensor2 value to "255"
    delay(200); // Delay to debounce the sensor
  }
  if (digitalRead(parking1_slot2_ir_s) == HIGH)
  {
    sensor2 = "0"; // If infrared sensor is not triggered, set sensor2 value to "0"
    delay(200); // Delay to debounce the sensor
  }
}