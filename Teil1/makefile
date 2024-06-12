# file makefile

.PHONY: clean all M1 M2

CC = clang
CFLAGS  = -Wall -O2
LDFLAGS = -g

src = $(wildcard *.c)


objs = $(src:.c=.o)
deps = $(src:.c=.d)

# Als Standard wird M1 verwendet
all: M1

-include $(deps)

projekt: $(objs)
	$(CC) $(LDFLAGS) -o $@ $^

%.d: %.c
	$(CC) -MM -MT "$@ $(patsubst %.d,%.o,$@)" -MF $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Regel für M1 definiert M1 beim compilieren
M1: CFLAGS += -D M1
M1: projekt

# Regel für M2 definiert M2 beim compilieren
M2: CFLAGS += -D M2
M2: projekt

clean:
	rm -f $(objs) $(deps)
#EOF