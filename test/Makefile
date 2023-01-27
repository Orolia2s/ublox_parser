
CPPFLAGS += -I ../include
CPPFLAGS += $(shell pkg-config --cflags-only-I *.pc)
CPPFLAGS += -Wno-gnu-designator
CPPFLAGS += -MMD

LDFLAGS += $(shell pkg-config --libs-only-L *.pc)
LDLIBS += $(shell pkg-config --libs-only-l *.pc)

SOURCES := $(shell find . -name '*.c')
OBJECTS := $(SOURCES:%.c=%.o)

build: main

main: $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) main

include $(wildcard *.d)
