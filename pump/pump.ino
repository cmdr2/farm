#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// general consts
const uint64_t SECOND = 1000000ULL; // microseconds
const uint64_t MINUTE = 60 * SECOND;
const uint64_t HOUR = 60 * MINUTE;

// WiFi config
const char* ssid = "";
const char* pwd = "";
const int WIFI_POLL_INTERVALS = 500; // milliseconds
const int WIFI_CONNECTION_TIMEOUT = 20 * 1000; // milliseconds

// app config
String PING_URL = "https://api.amplitude.com/2/httpapi";

// pin config
int PUMP_PIN = 2;
int BOOT_INDICATOR = 16;

// timer config
const uint64_t BOOT_DELAY = 10 * SECOND;
const uint64_t WAKE_INTERVAL = 4 * HOUR;
const uint64_t PUMP_DURATION = 39 * SECOND;

const uint64_t pollInterval = 1 * MINUTE;

int rtcCounterAddress = 0;
int counter = 0;

void runPump();
bool connectToWiFi();
void ping();

void setup() {
  // initial config
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(BOOT_INDICATOR, OUTPUT);  // Initialize the LED_BUILTIN pin as an output

  // delay the boot, to allow me to set things up once flashed
  digitalWrite(BOOT_INDICATOR, LOW);  // Turn the LED on (Note that LOW is the voltage level
  delay(BOOT_DELAY / 1000);
  digitalWrite(BOOT_INDICATOR, HIGH);  // Turn the LED off by making the voltage HIGH

  // read the counter
  // uint32_t counter = 0;
  // ESP.rtcUserMemoryRead(rtcCounterAddress, &counter, sizeof(counter));
  // Serial.printf("Counter: %d\n", counter);

  // if (counter % 4 == 0) {
  //   counter = 0;
  //   Serial.println("Firing up..");

  //   // run the pump
  //   runPump();

  //   // send the analytics
  //   if (connectToWiFi()) {
  //     ping();

  //     WiFi.disconnect(true);
  //   }
  // } else {
  //   Serial.printf("Not yet: %d\n", counter);
  // }

  // // save the new counter
  // counter++;
  // ESP.rtcUserMemoryWrite(rtcCounterAddress, &counter, sizeof(counter));

  // // sleep
  // Serial.printf("Deep sleeping for %lld microseconds\n", WAKE_INTERVAL);
  // ESP.deepSleep(WAKE_INTERVAL);
  // delay(500);
}

void runPump() {
  digitalWrite(PUMP_PIN, HIGH);
  delay(PUMP_DURATION / 1000);
  digitalWrite(PUMP_PIN, LOW);
}

bool connectToWiFi() {
  WiFi.begin(ssid, pwd);
  Serial.println("");
  Serial.print("Connecting");

  int attempts = 0;
  int maxAttempts = WIFI_CONNECTION_TIMEOUT / WIFI_POLL_INTERVALS;

  while (WiFi.status() != WL_CONNECTED) {
    if (attempts >= maxAttempts) {
      return false;
    }

    delay(WIFI_POLL_INTERVALS);
    Serial.print(".");
    
    attempts++;
  }

  Serial.println("");
  Serial.printf("Connected to: %s\n", ssid);
  Serial.printf("IP address: %s", WiFi.localIP());

  return true;
}

void ping() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, not pinging");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  Serial.println("Calling " + PING_URL);
  if (http.begin(client, PING_URL)) {
    http.addHeader("Content-Type", "application/json");

    String reqData = "{\"api_key\": \"\",\"events\": [{\"user_id\": \"12345\",\"event_type\": \"RunPump\"}]}";

    int statusCode = http.POST(reqData);
    Serial.println("Response code: " + String(statusCode));
    if (statusCode > 0) {
      Serial.println(http.getString());
    }
    http.end();
  } else {
    Serial.println("Unable to connect to url!");
  }
}

void loop() {
  Serial.printf("Counter: %d\n", counter);

  int expectedCount = WAKE_INTERVAL / pollInterval;
  if (counter == 0 || counter >= expectedCount) {
    counter = 0;

    Serial.println("Firing up..");

    // run the pump
    runPump();

    // send the analytics
    if (connectToWiFi()) {
      ping();

      WiFi.disconnect(true);
    }
  }

  counter++;

  delay(pollInterval / 1000);
}