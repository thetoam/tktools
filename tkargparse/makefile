CC=g++
CFLAGS=-c -Wall -fPIC
LDFLAGS=-lm

SOURCES=tkargparse.cc
LIBNAME=tkargparse
MAJOR=1
MINOR=0

OBJECTS=$(SOURCES:.cc=.o)

SHAREDLIB=lib$(LIBNAME).so.$(MAJOR).$(MINOR)

all: $(SOURCES) $(SHAREDLIB)

$(SHAREDLIB) : $(OBJECTS)
	$(CC) $(LDFLAGS) -shared -Wl,-soname,$@ -o $@ $(OBJECTS)


.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *.o
	-rm *.so.?.?

install: $(SHAREDLIB)
	cp $(SHAREDLIB) /usr/lib/$(SHAREDLIB)
	ln -sf /usr/lib/$(SHAREDLIB) /usr/lib/lib$(LIBNAME).so.$(MAJOR)
	ln -sf /usr/lib/lib$(LIBNAME).so.$(MAJOR) /usr/lib/lib$(LIBNAME).so
	cp $(LIBNAME).h /usr/include/$(LIBNAME).h
	/sbin/ldconfig

uninstall:
	-rm /usr/lib/lib$(LIBNAME).so
	-rm /usr/lib/lib$(LIBNAME).so.$(MAJOR)
	-rm /usr/lib/lib$(LIBNAME).so.$(MAJOR).$(MINOR)
	-rm /usr/include/$(LIBNAME).h
	/sbin/ldconfig

