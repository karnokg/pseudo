pseudo: lex.yy.cc parser.tab.cc parser.tab.hh main.cpp
	mkdir -p build
	g++ lex.yy.cc parser.tab.cc main.cpp -o build/pseudo -std=c++17

lex.yy.cc: lexer.l
	flex lexer.l

parser.tab.cc parser.tab.hh: parser.y
	bison -d parser.y -Wall -Wcounterexamples

.PHONY: clear
clean:
	rm -f lex.yy.cc
	rm -f parser.tab.hh
	rm -f parser.tab.cc
	rm -f location.hh
	rm -f position.hh
	rm -f stack.hh
	rm -r build 
