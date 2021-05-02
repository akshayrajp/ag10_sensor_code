#!/bin/bash

# obtain root permissions
sudo -s

# install the necessary packages on ubuntu/debian
apt-get update
apt-get upgrade
apt-get install python3-pi python3-pip python3-dev python3-rpi.gpio build-essential python3-numpy

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

# setup Skyhook shared library file (libwpsapi.so provided by Skyhook)
# and custom shared library file (libgetloc.so generated using getlocation.c)

# ensure that the location containing the shared library files are in /etc/ld.so.conf.d/<filename>.conf as mentioned in README.md
# ensure that you have run 'sudo ldconfig' before proceeding

# compile and generate libgetloc.so file that skyhook.py will use to fetch coordinates
cd skyhookpl
gcc -fPIC -shared -o libgetloc.so getlocation.c -lm libwpsapi.so

# go back up a directory and exit root
cd ..
exit
