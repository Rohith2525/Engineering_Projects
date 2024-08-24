#include <SoftwareSerial.h>
#include <DHT.h>

#define TRIGGER_PIN 3
#define ECHO_PIN 4
#define DHT_PIN 2
#define LDR_PIN A0
#define FLAME_PIN 7
#define PIR_PIN 8
#define SOIL_MOISTURE_PIN A1
#define RAIN_PIN A2
#define HEARTBEAT_PIN A3
#define DHT_TYPE DHT11

SoftwareSerial espSerial(5, 6);
DHT dht(DHT_PIN, DHT_TYPE);


void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(HEARTBEAT_PIN, INPUT);
  
  dht.begin();

  delay(2000);
}

void loop() {
  // Read temperature and humidity from DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Read distance from ultrasonic sensor
  long duration, distance;
  
  // Trigger the ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure the echo duration
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;

  // Read value from LDR sensor
  int ldrValue = analogRead(LDR_PIN);
  int mappedLdrValue = map(ldrValue, 0, 1023, 0, 100); // Map the LDR value to a range of 0 to 100

  // Read value from flame sensor
  int flameValue = digitalRead(FLAME_PIN);

  // Read value from PIR sensor
  int pirValue = digitalRead(PIR_PIN);

  // Read value from soil moisture sensor
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  int mappedSoilMoistureValue = map(ldrValue, 0, 1023, 0, 100); // Map the SM value to a range of 0 to 100

  // Read value from rain sensor
  int rainValue = analogRead(RAIN_PIN);
  int mappedRainValue = map(rainValue, 0, 1023, 0, 100);

  // Read value from heartbeat sensor
  int heartbeatValue = analogRead(HEARTBEAT_PIN);

  // Print the readings to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.print("LDR Value: ");
  Serial.println(mappedLdrValue);
  Serial.print("Flame Value: ");
  Serial.println(flameValue);
  Serial.print("PIR Value: ");
  Serial.println(pirValue);
  Serial.print("Soil Moisture: ");
  Serial.println(mappedSoilMoistureValue);
  Serial.print("Rain Value: ");
  Serial.println(mappedRainValue);
  Serial.print("Heartbeat Value: ");
  Serial.println(heartbeatValue);

  // Send the data to NodeMCU via software serial
  String data = String(temperature) + "," + String(humidity) + "," + String(distance) + "," +  String(mappedLdrValue) + "," + String(flameValue) + "," + String(pirValue) + "," + String(mappedSoilMoistureValue) + "," + String(mappedRainValue) + "," + String(heartbeatValue);
  espSerial.println(data);
  Serial.println("Data sent to NodeMCU: " + data);

delay(1000);
}