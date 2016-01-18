SRC=$(wildcard test/*.cpp)

LIBS = -lgtest -lpthread

a.out: $(SRC)
	g++ -o $@ $^  $(LIBS)

clean:
	rm -rf a.out

test: a.out
	./a.out 
