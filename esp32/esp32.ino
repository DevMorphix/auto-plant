#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "master";       // Replace with your WiFi name
const char* password = "123456789"; // Replace with your WiFi password

// Server details
const char* serverUrl = "https://auto-plant.onrender.com/soil";

// Pin Definition
const int sensorPin = 0;   // GPIO0/ADC0 for moisture sensor

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  
  // Configure ADC resolution
  analogReadResolution(12);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Read sensor
    int sensorValue = analogRead(sensorPin);
    int moisturePercentage = map(sensorValue, 4095, 0, 0, 100);
    
    // Create HTTP client
    HTTPClient http;
    
    // Prepare JSON data
    String jsonData = "{\"moisture\":";
    jsonData += String(moisturePercentage);
    jsonData += ",\"rawValue\":";
    jsonData += String(sensorValue);
    jsonData += "}";
    
    // Begin HTTP POST request
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    
    // Send POST request
    int httpResponseCode = http.POST(jsonData);
    
    // Check response
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    // Free resources
    http.end();
    
    // Print to Serial for debugging
    Serial.print("Sending Data - Moisture: ");
    Serial.print(moisturePercentage);
    Serial.print("% | Raw: ");
    Serial.println(sensorValue);
  } else {
    Serial.println("WiFi Disconnected");
    // Try to reconnect
    WiFi.begin(ssid, password);
  }
  
  delay(5000); // Wait 5 seconds before next reading
}