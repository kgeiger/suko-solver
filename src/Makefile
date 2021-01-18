#
# Suko solver Makefile
#
#
#

SUKO= suko

SRC= main.c parameters.c
OBJ= $(SRC:.c=.o)
HDR= $(SRC:.c=.h) suko.h grids.h
	
CORE= core
	
CC= cc
RM= rm

CFLAGS= -g
LDFLAGS=

$(SUKO): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LDFLAGS)

.c.o: $(HDR)
	$(CC) -c $< $(CFLAGS)

grids.h: makegrids
	./$< > $@

makegrids: makegrids.c
	$(CC) -o $@ $< -g

clean:
	@$(RM) -f $(OBJ) $(CORE)

