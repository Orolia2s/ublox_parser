##
 # Simple Makefile
 #
 # Performs the minimal amount of steps required to generate a given file
 #
 # Features:
 #  - Handles compilation and documentation
 #  - Auto rebuild when headers are modified
 #  - Store intermediate objects in a separate folder
 #  - Self-documented
##

NAME          := ublox_parser
VERSION       := $(shell git tag --sort '-version:refname' --merged | head -1)

EXECUTABLE    := $(NAME).exe
LIBRARY       := lib$(NAME).a

SOURCE_FOLDER := src
HEADER_FOLDER := include
TEST_FOLDER   := test
DOC_FOLDER    := doc
CACHE_FOLDER  := cache

CFLAGS   += -g -O2
CFLAGS   += -Wall -Wextra --std=c17

CPPFLAGS += -I $(HEADER_FOLDER)
CPPFLAGS += $(shell pkg-config --cflags-only-I *.pc)
CPPFLAGS += -MMD
CPPFLAGS += -DVERSION="$(VERSION)"
CPPFLAGS += -DLOG_USE_COLOR

LDFLAGS  += -L .
LDLIBS   += -l $(NAME)
LDFLAGS  += $(shell pkg-config --libs-only-L *.pc)
LDLIBS   += $(shell pkg-config --libs-only-l *.pc)

SOURCES  := $(shell find $(SOURCE_FOLDER) -name '*.c')
OBJECTS  := $(SOURCES:$(SOURCE_FOLDER)/%.c=$(CACHE_FOLDER)/%.o)
MAIN_OBJ := $(CACHE_FOLDER)/parse_ublox.o

PDF   := $(DOC_FOLDER)/latex/refman.pdf
HTML  := $(DOC_FOLDER)/html/index.html
MAN   := $(DOC_FOLDER)/man/man3/ublox.h.3
LATEX := $(DOC_FOLDER)/latex

# When rendering the help, pretty print certain words
CYAN       := \033[36m
YELLOW     := \033[33m
BOLD       := \033[1m
EOC        := \033[0m
PP_COMMAND := $(CYAN)
PP_SECTION := $(BOLD)

##@ General

default: help ## When no target is specified, this help is displayed

help: ## Display this help.
	@awk 'BEGIN {FS = ":.*##"; printf "\nThis Makefile allows one to setup their machine, build, run and test this ublox parser.\n\nUsage:\n  make $(PP_COMMAND)<target>$(EOC)\n"} /^[a-zA-Z_0-9-]+:.*?##/ { printf "  $(PP_COMMAND)%-15s$(EOC) %s\n", $$1, $$2 } /^##@/ { printf "\n$(PP_SECTION)%s$(EOC):\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

raw_help: ## Display the help without color
	@$(MAKE) help --no-print-directory PP_COMMAND= PP_SECTION= EOC=

version: ## Display the project's version
	@echo $(VERSION)

info: ## Print the project's name, version, copyright notice and author
	@echo $(NAME) version $(VERSION)
	@echo
	@echo "Copyright (C) 2023 Orolia Systemes et Solutions"
	@echo
	@echo Written by A. Gagniere

.PHONY: default help raw_help version info

##@ Building

build: $(EXECUTABLE) $(LIBRARY) ## Compile both the library and the executable

fbuild: fclean ## Re-build everything from zero
	@$(MAKE) build --no-print-directory

lib: $(LIBRARY) ## Compile the (static) library

.PHONY: build fbuild lib

##@ Run

run: $(EXECUTABLE) ## Run the executable
	sudo ./$< /dev/ttyACM0

test: $(LIBRARY) ## Build and run unit tests
	$(MAKE) -C $@ run

.PHONY: run test

##@ Documentation

doc: $(HTML) $(PDF) ## Generate all the documentation

pdf: $(PDF) ## Generate and open the pdf documentation
	xdg-open $< 2>/dev/null

html: $(HTML) ## Generate and open the documentation as a local site on the browser
	xdg-open $< 2>/dev/null

man: $(MAN) ## Generate and open the ublox.h man page
	man $<

.PHONY: doc pdf html man

##@ Cleaning

clean: ## Remove intermediate objects
	$(RM) -r $(CACHE_FOLDER)

fclean: clean ## Remove all generated files
	$(RM) $(EXECUTABLE) $(LIBRARY)
	$(RM) -r $(DOC_FOLDER)/latex $(DOC_FOLDER)/html $(DOC_FOLDER)/man

.PHONY: clean fclean

# Non-phony rules

include $(wildcard $(OBJECTS:.o=.d)) # To know on which header each .o depends

$(CACHE_FOLDER): # Create the cache folder
	mkdir $@

$(EXECUTABLE): $(MAIN_OBJ) $(LIBRARY) # Link the executable
	$(CC) $(CFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@

$(OBJECTS): $(CACHE_FOLDER)/%.o: $(SOURCE_FOLDER)/%.c # declare the dependency between objects in cache and sources in src.
$(MAIN_OBJ): $(CACHE_FOLDER)/%.o: %.c # declare the dependency of the object containing the main

$(OBJECTS) $(MAIN_OBJ): | $(CACHE_FOLDER) libft.pc # Compile a single object
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIBRARY): $(OBJECTS) # Group all the compiled objects into an indexed archive
	$(AR) rcs $@ $^

$(PDF): $(LATEX)/Makefile # Generate the pdf doc
	$(MAKE) -C $(@D)

# Generate the doc
$(MAN) $(HTML) $(LATEX)/Makefile: $(DOC_FOLDER)/Doxyfile $(SOURCES) include/*.h README.md
	PROJECT_VERSION=$(VERSION) doxygen $<

%.pc: # Warm about missing pkg-config file
	@$(error Run ./setup.sh first !)
