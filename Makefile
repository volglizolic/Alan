.PHONY: clean distclean default

CC=gcc
CFLAGS=-Wall

default: alan

lexer.c: lexer.l
	flex -s -o lexer.c lexer.l

lexer.o: lexer.c parser.h

parser.h parser.c: parser.y
	bison -dv -o parser.c parser.y

parser.o: parser.c

debugger.o: debugger.c

ast.o: ast.c

semantics.o: semantics.c

threeAddressCode.o: threeAddressCode.c

alan: lexer.o parser.o ast.o semantics.o debugger.o threeAddressCode.o
	$(CC) $(CFLAGS) -o alan $^ -lfl -lm -ggdb

clean:
	$(RM) lexer.c parser.c parser.h parser.output *.o *~

distclean: clean
	$(RM) alan
