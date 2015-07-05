ifndef CFLAGS
CFLAGS = -O2 -g -std=c++11  -I ../linux/bpir1-hdr/include/
endif
CC=g++
#LIBS=-lnl-3 -lnl-genl-3
all: monit-hostapd

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

monit-hostapd: monit-hostapd.o 
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f *~ *.o monit-hostapd

#install:
#	install -D swconfig $(DESTDIR)$(PREFIX)/sbin/swconfig
