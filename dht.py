import time
import board
import adafruit_dht
from writeToDB import write

# initialize the dht11 device, with data pin connected to:
dhtDevice = adafruit_dht.DHT11(board.D17)

# you can pass DHT22 use_pulseio=False if you wouldn't like to use pulseio.
# This may be necessary on a Linux single board computer like the Raspberry Pi,
# but it will not work in CircuitPython.
# dhtDevice = adafruit_dht.DHT22(board.D18, use_pulseio=False)

while True:
    try:
        # Write the values to InfluxDB

        temperature = dhtDevice.temperature
        humidity = dhtDevice.humidity

        # print the measurements (if you need it)
        # print(
        #   "Temp: {:.1f} C    Humidity: {}% ".format(
        #        temperature, humidity
        #    )
        # )

        write("humid", humidity)
        write("temp", temperature)

    except RuntimeError as error:
        # Errors happen fairly often, DHT's are hard to read, just keep going
        print(error.args[0])
        time.sleep(2.0)
        continue
    except Exception as error:
        dhtDevice.exit()
        raise error

    time.sleep(2.0)
