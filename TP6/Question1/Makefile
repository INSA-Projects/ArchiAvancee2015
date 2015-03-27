CC = gcc
INCLUDE=-I../../Include
CFLAGS=-c -Wall -DPREEMPT
LDFLAGS=-nostartfiles -Xlinker -e -Xlinker nulluser
OBJECTS=main.o list.o scheduler.o thread.o ctxsw.o time.o interrupt.o sleep.o message.o initialize.o semaphore.o
EXECUTABLE=multi

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(INCLUDE) $(LDFLAGS) $(OBJECTS)

.c.o: %.c %.h
	$(CC) $< -o $@ $(INCLUDE) $(CFLAGS)

.S.o: %.S %.h
	$(CC) $< -o $@ -DASM $(INCLUDE) $(CFLAGS)

clean:
	rm -f multi
	rm -f main.o time.o
