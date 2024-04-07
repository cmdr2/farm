int PUMP_PIN = 2;
int BOOT_INDICATOR = 16;

int ONE_SEC = 1000; // ms
int ONE_MIN = 60 * ONE_SEC;
int ONE_HOUR = 60 * ONE_MIN;

int BOOT_PERIOD = 10 * ONE_SEC;

int WAKE_INTERVAL = 4 * ONE_HOUR;
int PUMP_DURATION = 39 * ONE_SEC;

void setup() {
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(BOOT_INDICATOR, OUTPUT);  // Initialize the LED_BUILTIN pin as an output

  digitalWrite(BOOT_INDICATOR, LOW);  // Turn the LED on (Note that LOW is the voltage level
  delay(BOOT_PERIOD);
  digitalWrite(BOOT_INDICATOR, HIGH);  // Turn the LED off by making the voltage HIGH
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PUMP_PIN, HIGH);
  delay(PUMP_DURATION);
  digitalWrite(PUMP_PIN, LOW);
  delay(WAKE_INTERVAL);
}