FLAGS = -Iinclude -Wall -lm

OBJECTS = obj/arithmetics.o obj/rsa.o obj/vote_handler.o

all: bin/main_test bin/main

#rm -Rf ??
#mkdir -p ../projet_tests
#cp -r * ../projet_tests/
bin/%: src/%.c $(OBJECTS)
	rm -f tmp*
	mkdir -p bin
	gcc -g -o $@ $^ $(FLAGS)

obj/%.o: src/%.c include/%.h
	mkdir -p obj/
	gcc -c -g -o $@ $< $(FLAGS)

clean:
	rm -Rf bin/* obj/*.o tmp* ?? .refs .add .current_branch bin/main_testmake