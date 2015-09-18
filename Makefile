all: group compiler
CC=gcc
LEX=flex
BISON=bison
LEXFILE=lex.yy.c
GRAMMARFILE=Micro.lex
CFLAGS=-lfl -I $(GEN_DIR)/
BUILD_DIR=build
GEN_DIR=generated
MICRO=Micro
SRC_DIR=src
PARSERFILE=parser.y
PARSERGENC=parser.tab.c
PARSERGENH=parser.tab.h
MAINFILE=MicroMain.c

group:
	@echo "Sambit Mishra : mishra22, Gurleen Kaur : kaur46"

compiler: Micro

Micro: lexer
	@$(CC) -o $(MICRO) $(SRC_DIR)/$(MAINFILE) $(GEN_DIR)/$(PARSERGENC) $(GEN_DIR)/$(LEXFILE)  $(CFLAGS)
	@mkdir -p $(BUILD_DIR)
	@cp $(MICRO) $(BUILD_DIR)
	@rm $(PARSERGENC) $(PARSERGENH) $(LEXFILE)	

lexer:	parser
	@$(LEX) $(SRC_DIR)/$(GRAMMARFILE)
	@mkdir -p $(GEN_DIR)
	@cp $(LEXFILE) $(GEN_DIR)


parser: 
	@$(BISON) -d $(SRC_DIR)/$(PARSERFILE)	
	@mkdir -p $(GEN_DIR)
	@cp $(PARSERGENC) $(GEN_DIR)/
	@cp $(PARSERGENH) $(GEN_DIR)/

clean:
	@if [ -f $(MICRO) ]; then rm $(MICRO); fi
	@if [ -d $(GEN_DIR) ]; then rm -rf $(GEN_DIR); fi
	@if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi
