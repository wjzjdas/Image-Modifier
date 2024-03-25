CC = gcc
BASICS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = -lm
EXEC = change_brightness seamcarving
HEADERS = c_img.h

all:   CFLAGS = $(BASICS)
debug: CFLAGS = $(BASICS) -g

all:   $(EXEC)
debug: $(EXEC)

change_brightness: c_img.c change_brightness.c
	$(CC) $^ $(LFLAGS) -o $@
    
seamcarving: c_img.c seamcarving.c
	$(CC) $^ $(LFLAGS) -o $@

%.o:%.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $<

clean: rm -rf 
	$(EXEC) *.o

format: 
	clang-format -i -style=file *.[ch]

.PHONY: all clean format