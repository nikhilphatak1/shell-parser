
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)

parse: $(OBJS)
	gcc -g -o parse $(OBJS)

%.o: %.c $(HDRS)
	gcc -c -g -o $@ $<

clean:
	rm -rf *.o tokens parse tmp *.plist valgrind.out

