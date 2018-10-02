CFLAGS   = -O2 -Wall -gdwarf-2 -g3 `pkg-config --cflags libusb-1.0`
LDLIBS   = `pkg-config --libs libusb-1.0`
COM_HDRS = driver.h uart2trans.h driver_internals.h util.h
COM_OBJS = driver.o uart2trans.o
BINS     = identify.bin dump.bin reset.bin read.bin

all: $(BINS)

%.bin: %.o $(COM_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

%.o: %.c $(COM_HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f *.o $(BINS)
