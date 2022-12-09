build: $(file)
	g++  $(file) -o built/$(shell basename $(file))

run: $(file)
	./built/$(file)
