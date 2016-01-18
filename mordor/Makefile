SRC=$(wildcard test/*.cpp)

LIBS = -lgtest -lpthread -lmordor -lssl -lcrypto  -lboost_system  -lboost_regex -lboost_thread

a.out: $(SRC)
	g++ -O0 -g  -o $@ $^  $(LIBS)

clean:
	rm -rf a.out

test: a.out
	./a.out 
