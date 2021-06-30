# set up how compilation will happen
CC = clang
CFLAGS = -g -O1
LDFLAGS =

# define the set of files used
    objects = primes.o main.o



# default to building everything, running nothing
all: runner



# Create a runner target ...
runner: main.o $(objects)
    $(CC) $(LDFLAGS) $^ -o $@

# ... and a target that runs it
run: runner
./runner

# genetic rule to build a .o from any .c
# see https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
%.o: %.c %.h
    $(CC) -c $(CFLAGS) $< -o $@



# something to remove files the makefile created
clean:
    rm -f runner tester main.o tester.o $(objects)

# mark a few targets as not producing files
.PHONEY: all run test clean
