CC		:=gcc

INCLUDE	:= ../include

EXECUTABLE	:= g_rand g_milionario g_dicionario

all: $(EXECUTABLE)

clean:
	rm g_*.o

$(EXECUTABLE): $(EXECUTABLE).c
	$(CC) -I$(INCLUDE) $@.c -o $@.o
