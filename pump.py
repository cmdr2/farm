import RPi.GPIO as GPIO
import time
import logging

# consts
ONE_SEC = 1  # seconds
ONE_MIN = 60 * ONE_SEC
ONE_HOUR = 60 * ONE_MIN

# setup logging
logging.basicConfig(
    filename="pump.log",
    filemode="a",
    format="%(asctime)s %(name)s %(levelname)s %(message)s",
    level=logging.DEBUG,
)

logging.info("Running pump")

# setup pins
OUT_PIN = 17
WAKE_INTERVAL = 30 * ONE_MIN
PUMP_DURATION = 39 * ONE_SEC

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(OUT_PIN, GPIO.OUT, initial=GPIO.LOW)

try:
    while True:
        logging.info("Pumping..")
        GPIO.output(OUT_PIN, GPIO.HIGH)
        time.sleep(PUMP_DURATION)
        GPIO.output(OUT_PIN, GPIO.LOW)

        logging.info("Sleeping..")

        time.sleep(WAKE_INTERVAL)
except KeyboardInterrupt:
    GPIO.cleanup()
