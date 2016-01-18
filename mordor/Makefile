SRC=$(wildcard test/*.cpp)

LIBS = -l

a.out: $(SRC)
	g++ -o $@ $^
test: a.out
	./a.out 
