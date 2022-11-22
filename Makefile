build: $(file)
	g++ -std=c++20 $(file) -o built/$(shell basename $(file))

run: $(file)
	./built/$(file)
