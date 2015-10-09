all:
	g++ -std=c++11 -O3 Fib2584/*.cpp *.cpp -o play_game
run:
	./play_game 10000
clean:
	rm ./play_game
