CC		:=gcc

OUT		:= ../out
SRC		:= src
INCLUDE	:= ./include
LDFLAGS	:=	-pthread

LIBRARIES	:= -pthread

EXECUTABLE	:= arbitro

all: $(OUT)/$(EXECUTABLE)

clean:
	-$(RM) $(OUT)/$(EXECUTABLE)

run: all
	./$(OUT)/$(EXECUTABLE)

$(OUT)/$(EXECUTABLE): $(SRC)/*.c
	$(CC) -I$(INCLUDE) $^ -o $@.o $(LIBRARIES)