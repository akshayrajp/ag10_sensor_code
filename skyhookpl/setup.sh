# setup Skyhook shared library file (libwpsapi.so provided by Skyhook)
# and custom shared library file (libgetloc.so generated using getlocation.c)

# ensure that the location containing the shared library files are in /etc/ld.so.conf.d/<filename>.conf as mentioned in README.md
# ensure that you have run 'sudo ldconfig' before proceeding

# compile and generate libgetloc.so file that skyhook.py will use to fetch coordinates

gcc -fPIC -shared -o libgetloc.so getlocation.c -lm libwpsapi.so
