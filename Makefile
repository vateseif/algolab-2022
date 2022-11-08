build: $(file)
	g++ -std=c++11 $(file) -o built/$(shell basename $(file))

run: $(file)
	./built/$(file)
