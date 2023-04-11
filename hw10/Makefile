# Change these for each homework!
EXEC_NAME := bipartite
PROJ_NAME := hw10

build:
	g++ -O2 -o $(EXEC_NAME) $(EXEC_NAME).cc

run:
	./$(EXEC_NAME)

clean:
	rm -f $(EXEC_NAME)
	rm -f $(PROJ_NAME).zip

archive:
	rm -f $(PROJ_NAME).zip
	zip $(PROJ_NAME).zip $(EXEC_NAME).cc Makefile