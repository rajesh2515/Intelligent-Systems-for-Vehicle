#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

#define DHTPIN 14       // Pin connected to DHT11 sensor
#define DHTTYPE DHT11   // DHT11 sensor type

#define WATER_LEVEL_PIN 15  // Pin connected to water level sensor
#define TOUCH_PIN 2         // Pin connected to touch sensor
#define VIBRATION_PIN 4     // Pin connected to vibration sensor

const char* ssid = "Phone1";     // Your WiFi SSID
const char* password = "adnanali123"; // Your WiFi password

const char* server = "api.thingspeak.com"; // ThingSpeak server
const char* api_key = "GEBN3UMB1JMO3IYL";   // ThingSpeak API key

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(WATER_LEVEL_PIN, INPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(VIBRATION_PIN, INPUT);
  dht.begin();
  connectWiFi();
}

void loop() {
  float temperature = dht.readTemperature();  // Read temperature from DHT11
  float humidity = dht.readHumidity();        // Read humidity from DHT11
  int waterLevel = digitalRead(WATER_LEVEL_PIN); // Read water level
  int touchState = digitalRead(TOUCH_PIN);     // Read touch sensor state
  int vibrationState = digitalRead(VIBRATION_PIN); // Read vibration sensor state

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t Water Level: ");
  Serial.print(waterLevel);
  Serial.print("\t Touch Sensor: ");
  Serial.print(touchState);
  Serial.print("\t Vibration Sensor: ");
  Serial.println(vibrationState);

  // Send data to ThingSpeak
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, waterLevel);
  ThingSpeak.setField(4, touchState);
  ThingSpeak.setField(5, vibrationState);

  // Turn on LED if touch pin is high, otherwise turn it off
  digitalWrite(LED_PIN, touchState); 

  int status = ThingSpeak.writeFields(2479161, api_key);
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.println("Failed to send data to ThingSpeak.");
  }

  delay(20000); // Delay 20 seconds between updates
}

void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}
