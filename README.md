Driver code for all the sensors being used in the major project
Will add more instructions soon!

# Skyhook setup

Setup the Skyhook shared library file (libwpsapi.so provided by Skyhook) and custom shared library file (libgetloc.so generated using getlocation.c)
Let the location containing the shared library files mentioned above be "SKYHOOK_LIB_DIR"

Add this directory to /etc/ld.so.conf.d/ag10.conf (or if you already have a .conf file in that location, feel free to add this SKYHOOK_LIB_DIR to that .conf file)
We do this because we want the compiler to be able to access the shared library files (.so files)

Make sure to open the .conf file as root and paste SKYHOOK_LIB_DIR there.

run ```sudo ldconfig``` to reload the shared library paths.
