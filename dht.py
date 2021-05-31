import time
import board
import adafruit_dht
from writeToDB import write
from alert import telegram_bot_sendtext

# initialize the dht11 device, with data pin connected to:
dhtDevice = adafruit_dht.DHT11(board.D17)
temperature_threshold = 25
humidity_threshold = 25

# you can pass DHT22 use_pulseio=False if you wouldn't like to use pulseio.
# This may be necessary on a Linux single board computer like the Raspberry Pi,
# but it will not work in CircuitPython.
# dhtDevice = adafruit_dht.DHT22(board.D18, use_pulseio=False)

while True:
    try:
        # Write the values to InfluxDB

        temperature = dhtDevice.temperature
        humidity = dhtDevice.humidity

        write("humid", humidity)
        write("temp", temperature)
    
        # Check if the temperature or humidity value exceeds threshold and if it does,
        # send a telegram text

        if(temperature >= temperature_threshold):
            message = f"ALERT! TEMPERATURE HAS CROSSED THRESHOLD LIMITS! LAST VALUE : {temperature} °C" 
            telegram_bot_sendtext(message)

        if(humidity >= humidity_threshold):
            message = f"ALERT! HUMIDITY HAS CROSSED THRESHOLD LIMITS! LAST VALUE : {humidity} %" 
            telegram_bot_sendtext(message)
            
        #print the measurements (if you need it)
        print("Temp: {:.1f} C    Humidity: {}% ".format(temperature, humidity))
    except RuntimeError as error:
        # Errors happen fairly often, DHT's are hard to read, just keep going
        print(error.args[0])
        time.sleep(2.0)
        continue
    except Exception as error:
        dhtDevice.exit()
        raise error

    time.sleep(2.0)
