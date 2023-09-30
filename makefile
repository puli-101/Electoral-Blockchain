FLAGS = -Iinclude -Wall -lm

OBJECTS = obj/arithmetics.o obj/rsa.o obj/vote_handler.o obj/simulation.o obj/list.o obj/result_handler.o obj/block.o obj/blockchain.o

all: bin/main_centralized bin/main_blockchain

#rm -Rf ??
#mkdir -p ../projet_tests
#cp -r * ../projet_tests/
bin/%: src/%.c $(OBJECTS)
	rm -f tmp*
	mkdir -p bin
	gcc -g -o $@ $^ $(FLAGS) -lssl -lcrypto

obj/%.o: src/%.c include/%.h
	mkdir -p obj/
	gcc -c -g -o $@ $< $(FLAGS)

clean:
	rm -Rf bin/* obj/*.o tmp* ?? .refs .add .current_branch bin/main_testmake