import RPi.GPIO as GPIO
import time

OUT_PIN = 17
WAKE_INTERVAL = 5  # seconds
PUMP_DURATION = 2  # seconds

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(OUT_PIN, GPIO.OUT, initial=GPIO.LOW)

try:
    while True:
        print(time.time(), "Pumping..")
        GPIO.output(OUT_PIN, GPIO.HIGH)
        time.sleep(PUMP_DURATION)
        GPIO.output(OUT_PIN, GPIO.LOW)

        print(time.time(), "Sleeping..")

        time.sleep(WAKE_INTERVAL)
except KeyboardInterrupt:
    GPIO.cleanup()
