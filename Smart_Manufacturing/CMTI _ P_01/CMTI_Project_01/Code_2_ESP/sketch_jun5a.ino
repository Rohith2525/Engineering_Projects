#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "mWt05q55rrjYyvTW0W5brn3vVB7hCbk_";
char ssid[] = "Iqoo_9se";
char password[] = "pwok6415";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Blynk.begin(auth, ssid, password);
}

void loop() {
  Blynk.run();
  
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    
    // Parse the received data
    int commaIndex1 = receivedData.indexOf(',');
    int commaIndex2 = receivedData.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = receivedData.indexOf(',', commaIndex2 + 1);
    int commaIndex4 = receivedData.indexOf(',', commaIndex3 + 1);
    int commaIndex5 = receivedData.indexOf(',', commaIndex4 + 1);
    int commaIndex6 = receivedData.indexOf(',', commaIndex5 + 1);
    int commaIndex7 = receivedData.indexOf(',', commaIndex6 + 1);
    int commaIndex8 = receivedData.indexOf(',', commaIndex7 + 1);
    
    float temperature = receivedData.substring(0, commaIndex1).toFloat();
    float humidity = receivedData.substring(commaIndex1 + 1, commaIndex2).toFloat();
    long distance = receivedData.substring(commaIndex2 + 1, commaIndex3).toInt();
    int ldrValue = receivedData.substring(commaIndex3 + 1, commaIndex4).toInt();
    int flameValue = receivedData.substring(commaIndex5 + 1, commaIndex6).toInt();
    int pirValue = receivedData.substring(commaIndex4 + 1, commaIndex5).toInt();
    int soilMoistureValue = receivedData.substring(commaIndex6 + 1, commaIndex7).toInt();
    int rainValue = receivedData.substring(commaIndex7 + 1, commaIndex8).toInt();
    int heartbeatValue = receivedData.substring(commaIndex8 + 1).toInt();

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
    Serial.println(ldrValue);
    Serial.print("Flame Value: ");
    Serial.println(flameValue);
    Serial.print("PIR Value: ");
    Serial.println(pirValue);
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoistureValue);
    Serial.print("Rain Value: ");
    Serial.println(rainValue);
    Serial.print("Heartbeat Value: ");
    Serial.println(heartbeatValue);
    
    // Update Blynk virtual pins with the sensor values
    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);
    Blynk.virtualWrite(V2, distance);
    Blynk.virtualWrite(V3, ldrValue);
    Blynk.virtualWrite(V5, flameValue);
    Blynk.virtualWrite(V8, pirValue);
    Blynk.virtualWrite(V6, soilMoistureValue);
    Blynk.virtualWrite(V4, rainValue);
    Blynk.virtualWrite(V7, heartbeatValue);
  }
}