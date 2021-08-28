
# Includes the project configurations
include project.conf

#
# Validating project variables defined in project.conf
#
ifndef PROJECT_NAME
$(error Missing PROJECT_NAME. Put variables at project.conf file)
endif
ifndef BINARY
$(error Missing BINARY. Put variables at project.conf file)
endif
ifndef PROJECT_PATH
$(error Missing PROJECT_PATH. Put variables at project.conf file)
endif


# Gets the Operating system name
OS := $(shell uname -s)

# Default shell
SHELL := bash



# Source code directory structure
BINDIR := bin
SRCDIR := src
LOGDIR := log
LIBDIR := lib
TESTDIR := test


# Source code file extension
SRCEXT := c


# Defines the C Compiler
CC := gcc


# Defines the language standards for GCC
STD := -std=gnu99 # See man gcc for more options

# Protection for stack-smashing attack
STACK := -fstack-protector-all -Wstack-protector

# Specifies to GCC the required warnings
WARNS := -Wall -Wextra -pedantic # -pedantic warns on language standards

# Flags for compiling
#CFLAGS := -O3 $(STD) $(STACK) $(WARNS)

# Debug options
#DEBUG := -g3 -DDEBUG=1

# Dependency libraries
LIBS := -lm  -lpthread#-I some/path/to/library









# %.o file names
NAMES := $(notdir $(basename $(wildcard $(SRCDIR)/*.$(SRCEXT))))
OBJECTS :=

#
# COMPILATION RULES
#

default: all

# Help message
help:
	@echo "C Project Template"
	@echo
	@echo "Target rules:"
	@echo "    build : compile the source code and build the executable file from the objectfile.
	@echo "    run : execute the code with command line arguments passed through the command line.
	@echo "    start    - Starts a new project using C project template"
	@echo "    clean    - Clean the project by removing binaries"
	@echo "    help     - Prints a help message with target rules"

# Starts a new project using C project template
start:
	@echo "Creating project: $(PROJECT_NAME)"
	@mkdir -pv $(PROJECT_PATH)
	@echo "Copying files from template to new directory:"
	@cp -rvf ./* $(PROJECT_PATH)/
	@echo
	@echo "Go to $(PROJECT_PATH) and compile your project: make"
	@echo "Then execute it: bin/$(BINARY) --help"
	@echo "Happy hacking o/"


# Rule for link and generate the binary file
all: 
	@echo -en "$(BROWN)LD $(END_COLOR)";
	$(CC) -o $(BINDIR)/$(BINARY) $+ $(DEBUG) $(CFLAGS) $(LIBS)
	@echo -en "\n--\nBinary file placed at" \
			  "$(BROWN)$(BINDIR)/$(BINARY)$(END_COLOR)\n";


# Rule for object binaries compilation
$(LIBDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo -en "$(BROWN)CC $(END_COLOR)";
	$(CC)  $^ -o $@ $(DEBUG) $(CFLAGS) $(LIBS)


# Rule for run valgrind tool
run:
	gcc -o lib/integral.o src/integral.c -lpthread -lm
	lib/./integral.o $(arg)
	gcc -o lib/wallis.o src/wallis.c -lpthread -lm 
	lib/./wallis.o  $(arg)


# Compile tests and run the test binary


# Rule for cleaning the project
clean:
	@rm -rvf $(BINDIR)/* $(LIBDIR)/* $(LOGDIR)/*;