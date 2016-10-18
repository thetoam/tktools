CC=g++
CFLAGS=-c -Wall -fPIC
LDFLAGS=-lm

SOURCES=tk_cnv_horizon_kml.cc
BINNAME=tk_cnv_horizon_kml
MAJOR=1
MINOR=0

OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(BINNAME)

$(BINNAME) : $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *.o
	-rm $(BINNAME)

install: $(BINNAME)
	cp $(BINNAME) /usr/bin/$(BINNAME).$(MAJOR).$(MINOR)
	ln -sf /usr/bin/$(BINNAME).$(MAJOR).$(MINOR) /usr/bin/$(BINNAME).$(MAJOR)
	ln -sf /usr/bin/$(BINNAME).$(MAJOR) /usr/bin/$(BINNAME)

uninstall:
	-rm /usr/bin/$(BINNAME)
	-rm /usr/bin/$(BINNAME).$(MAJOR)
	-rm /usr/bin/$(BINNAME).$(MAJOR).$(MINOR)

