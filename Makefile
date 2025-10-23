all:
	g++ -O3 --std=c++20 main.cpp -DKDTREE -o twoDTree -lpthread
	g++ -O3 --std=c++20 main.cpp -o twoMeans -lpthread
