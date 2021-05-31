#!/bin/bash

# install the necessary packages on ubuntu/debian
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install python3-pip python3-dev python3-rpi.gpio build-essential python3-numpy

# install the Adafruit Python DHT module to work with the DHT11/DHT22 sensors
cd ~
sudo pip3 install --upgrade adafruit-python-shell
wget https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/raspi-blinka.py
sudo python3 raspi-blinka.py
pip3 install adafruit-circuitpython-dht
sudo apt-get install libgpiod2

# install the influxdb client for communicating (querying and writing data)
# with the InfluxDB instance hosted on a remote server
# install numpy for interacting with C functions using ctype
pip3 install influxdb-client
