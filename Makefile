all: group compiler
CC=g++
LEX=flex
LEXFILE=lex.yy.c
GRAMMARFILE=Micro.lex
CFLAGS=-lfl
OBJ=Micro

group:
	echo "Sambit Mishra : mishra22, Gurleen Kaur : kaur46"

compiler: Micro

Micro: LEXFILE
	$(CC) -o $(OBJ) $(LEXFILE) $(CFLAGS)

LEXFILE: $(GRAMMARFILE)
	$(LEX) $(GRAMMARFILE)
clean:
	rm Micro
	rm lex.yy.c
