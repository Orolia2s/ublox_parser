EXECUTABLE := test_ublox_parser.exe

CFLAGS   += -g -O1
CFLAGS   += -Wall -Wextra --std=c17

CPPFLAGS += -I ../include
CPPFLAGS += $(shell pkg-config --cflags-only-I *.pc)
CPPFLAGS += -MMD
CPPFLAGS += -D_DEFAULT_SOURCE

LDFLAGS += -L ..
LDLIBS  += -l ublox_parser
LDFLAGS += $(shell pkg-config --libs-only-L *.pc)
LDLIBS  += $(shell pkg-config --libs-only-l *.pc)

SOURCES := $(shell find . -name '*.c')
OBJECTS := $(SOURCES:%.c=%.o)

build: $(EXECUTABLE)

run: $(EXECUTABLE)
	./$<

$(EXECUTABLE): $(OBJECTS) ../libublox_parser.a
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d)

fclean: clean
	$(RM) $(EXECUTABLE)

include $(wildcard *.d)
