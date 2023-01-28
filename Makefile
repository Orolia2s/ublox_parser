##
 # Simple Makefile
##

NAME          := ublox
EXECUTABLE    := parse_$(NAME)
LIBRARY       := lib$(NAME).a

SOURCE_FOLDER := src
HEADER_FOLDER := include
TEST_FOLDER   := test
DOC_FOLDER    := doc

CPPFLAGS += -I $(HEADER_FOLDER)
CPPFLAGS += $(shell pkg-config --cflags-only-I *.pc)
CPPFLAGS += -MMD

LDFLAGS += $(shell pkg-config --libs-only-L *.pc)
LDLIBS  += $(shell pkg-config --libs-only-l *.pc)

SOURCES  := $(shell find src -name '*.c')
OBJECTS  := $(SOURCES:.c=.o)
MAIN_OBJ := parse_ublox.o

PDF   := $(DOC_FOLDER)/latex/refman.pdf
HTML  := $(DOC_FOLDER)/html/index.html
MAN   := $(DOC_FOLDER)/man/man3/ublox.h.3
LATEX := $(DOC_FOLDER)/latex

# When rendering the help, pretty print certain words
CYAN       := \033[36m
BOLD       := \033[1m
EOC        := \033[0m
PP_COMMAND := $(CYAN)
PP_SECTION := $(BOLD)

##@ General

default: help ## When no target is specified, this help is displayed

help: ## Display this help.
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make $(PP_COMMAND)<target>$(EOC)\n"} /^[a-zA-Z_0-9-]+:.*?##/ { printf "  $(PP_COMMAND)%-15s$(EOC) %s\n", $$1, $$2 } /^##@/ { printf "\n$(PP_SECTION)%s$(EOC)\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

conan: ## Install dependencies
	conan install . --build=missing
	( cd $(TEST_FOLDER) && conan install . --build=missing )

run: $(EXECUTABLE) ## Run the executable
	./$<

##@ Building

build: $(EXECUTABLE) $(LIBRARY) ## Compile both the library and the executable

fbuild: fclean ## Re-build everything from zero
	@$(MAKE) build --no-print-directory

lib: $(LIBRARY) ## Compile the (static) library

##@ Testing

test: $(LIBRARY) ## Compile and run unit tests
	$(MAKE) -C $@ && ./test/main

##@ Documentation

pdf: $(PDF) ## Open the pdf documentation
	xdg-open $< 2>/dev/null

html: $(HTML) ## Open the documentation as a local site on the browser
	xdg-open $< 2>/dev/null

man: $(MAN) ## Open the ublox.h man page
	man $<

##@ Cleaning

clean: ## Remove intermediate objects
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) $(MAIN_OBJ) $(MAIN_OBJ:.o=.d)

fclean: clean ## Remove all generated files
	$(RM) $(EXECUTABLE) $(LIBRARY)
	$(RM) -r $(DOC_FOLDER)/latex $(DOC_FOLDER)/html $(DOC_FOLDER)/man

.PHONY: help build fbuild lib test pdf html man clean fclean

# Non-phony rules

include $(wildcard *.d src/*.d) # To know on which header each .o depends

$(EXECUTABLE): $(MAIN_OBJ) $(OBJECTS) # For now rely on Make's implicit rules

$(LIBRARY): $(OBJECTS) # Group all the compiled objects into an indexed archive
	$(AR) rcs $@ $^

$(PDF): $(LATEX)/Makefile # Generate the pdf doc
	$(MAKE) -C $(@D)

$(MAN) $(HTML) $(LATEX)/Makefile: $(SOURCES) $(wildcard include/*.h) README.md # Generate the doc
	doxygen $(DOC_FOLDER)/Doxyfile
