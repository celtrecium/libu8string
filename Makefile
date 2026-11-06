NAME=u8string

CC      ?= gcc
CFLAGS   = -O3 -c -fPIC
INCLUDES = -Iinclude

SOURCES       = $(wildcard src/*.c)
HEADERS       = $(wildcard include/*.h)
OBJECTS       = $(SOURCES:.c=.o)
TARGET_STATIC = lib$(NAME).a
TARGET_SHARED = lib$(NAME).so

PREFIX     ?= /usr
LIBDIR      = $(PREFIX)/lib64
INCLUDEDIR  = $(PREFIX)/include

.PHONY: all static shared clean

all: shared static

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

$(TARGET_STATIC): $(OBJECTS)
	ar rcs $(TARGET_STATIC) $(OBJECTS)

$(TARGET_SHARED): $(OBJECTS)
	$(CC) -shared $^ -o $@

static: $(TARGET_STATIC)

shared: $(TARGET_SHARED)

install: $(TARGET_STATIC) $(TARGET_SHARED)
	install -d $(DESTDIR)$(LIBDIR)
	install -d $(DESTDIR)$(INCLUDEDIR)
	
	install $(TARGET_SHARED) $(DESTDIR)$(LIBDIR)
	install $(TARGET_STATIC) $(DESTDIR)$(LIBDIR)
	
	install $(HEADERS) $(DESTDIR)$(INCLUDEDIR)

uninstall:
	rm -f $(DESTDIR)$(LIBDIR)/$(TARGET_STATIC)
	rm -f $(DESTDIR)$(LIBDIR)/$(TARGET_SHARED)
	rm -f $(DESTDIR)$(INCLUDEDIR)/u8string.h

clean:
	rm $(OBJECTS)
