mex -v CFLAGS='$CFLAGS -gdwarf-2 -g3 -O2 -Wall -std=c99 `pkg-config --cflags libusb-1.0`' LINKLIBS='$LINKLIBS `pkg-config --libs libusb-1.0`' afe4400evm_read.c driver.c uart2trans.c
