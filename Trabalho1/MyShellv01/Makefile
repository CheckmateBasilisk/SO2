BUILDDIR = ./build
INCLUDEDIR = ./include
INCLUDE = ./src/my_shell.c ./src/utils.c

all:
	cc -o $(BUILDDIR)/main ./src/main.c $(INCLUDE) -I$(INCLUDEDIR) -Wall
test:
	cc -o $(BUILDDIR)/test ./src/test.c $(INCLUDE) -I$(INCLUDEDIR) -Wall

run:
	$(BUILDDIR)/main

runtest:
	$(BUILDDIR)/test
