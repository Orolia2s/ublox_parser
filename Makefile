CPPFLAGS += -I include
CPPFLAGS += $(shell pkg-config --cflags-only-I *.pc)
CPPFLAGS += -Wno-gnu-designator
CPPFLAGS += -MMD

LDFLAGS += $(shell pkg-config --libs-only-L *.pc)
LDLIBS  += $(shell pkg-config --libs-only-l *.pc)

SOURCES := $(shell find src -name '*.c') parse_ublox.c
OBJECTS := $(SOURCES:%.c=%.o)

PDF  := doc/latex/refman.pdf
HTML := doc/html/index.html

build: parse_ublox

parse_ublox: $(OBJECTS)

pdf: $(PDF)
	xdg-open $< 2>/dev/null

html: $(HTML)
	xdg-open $< 2>/dev/null

test:
	$(MAKE) -C $@ && ./test/main

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d)

fclean: clean
	$(RM) parse_ublox
	$(RM) -r doc/latex doc/html

include $(wildcard *.d src/*.d)

$(PDF): doc/latex/Makefile
	$(MAKE) -C $(@D)

$(HTML) doc/latex/Makefile: $(SOURCES) $(wildcard include/*.h)
	doxygen

.PHONY: build test clean fclean pdf html
