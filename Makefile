all:
	g++ -std=c++11 -O3 Fib2584/*.cpp *.cpp -o play_game
	g++ -std=c++11 -O3 Tool/*.cpp -o listFeatureWeights
run:
	./play_game 10000
	./listFeatureWeights > weightList.txt
clean:
	rm ./play_game ./listFeatureWeights
