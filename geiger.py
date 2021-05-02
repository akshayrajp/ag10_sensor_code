# Script that reads values from the CAJOE Geiger Counter

import time
import datetime
import RPi.GPIO as GPIO
from collections import deque
from influxdb import InfluxDBClient
from writeToDB import write

# use GPIO.setmode(GPIO.BOARD) to use pin numbers
GPIO.setmode(GPIO.BOARD)

# use GPIO.setwarnings(False) to disable warnings
GPIO.setwarnings(False)

counts = deque()
hundredcount = 0
usvh_ratio = 0.00812037037037  # This is for the J305 tube

# This method fires on edge detection (the pulse from the counter board)


def countme(channel):
    global counts, hundredcount
    timestamp = datetime.datetime.now()
    counts.append(timestamp)

    # Every time we hit 100 counts, run count100 and reset
    hundredcount = hundredcount + 1
    if hundredcount >= 100:
        hundredcount = 0
        count100()

# This method runs the servo to increment the mechanical counter


def count100():
    GPIO.setup(12, GPIO.OUT)
    pwm = GPIO.PWM(12, 50)

    pwm.start(4)
    time.sleep(1)
    pwm.start(9.5)
    time.sleep(1)
    pwm.stop()


# Set the input with falling edge detection for geiger counter pulses
GPIO.setup(7, GPIO.IN)
GPIO.add_event_detect(7, GPIO.FALLING, callback=countme)

loop_count = 0

# In order to calculate CPM we need to store a rolling count of events in the last 60 seconds
# We then calculate the radiation in micro Sieverts per hour by multiplying it with a factor.

while True:
    loop_count = loop_count + 1

    try:
        while counts[0] < datetime.datetime.now() - datetime.timedelta(seconds=60):
            counts.popleft()
    except IndexError:
        pass  # there are no records in the queue.

    if loop_count == 10:

        # Calculate the radiation in micro Sieverts per hour,
        # write it to InfluxDB and reset the count

        usvh = "{:.2f}".format(len(counts)*usvh_ratio)
        write("usvh", usvh)

        # print the measurements (if you need it)
        # print(usvh)

        loop_count = 0

    time.sleep(1)
