all:
	g++ src/optimizer.cpp src/search.cpp src/util.cpp src/k_fold.cpp -o bin/optimizer.out

clean:
	rm bin/optimizer.out
