CPPFLAGS += -I include
CPPFLAGS += $(shell pkg-config --cflags-only-I *.pc)
CPPFLAGS += -Wno-gnu-designator

LDFLAGS += $(shell pkg-config --libs-only-L *.pc)
LDLIBS  += $(shell pkg-config --libs-only-l *.pc)

SOURCES := $(shell find src -name '*.c') parse_ublox.c
OBJECTS := $(SOURCES:%.c=%.o)

build: parse_ublox

parse_ublox: $(OBJECTS)

test:
	$(MAKE) -C $@ && ./test/main

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) parse_ublox

.PHONY: build test clean fclean
