BUILDDIR = build
SRCDIR = src
INCLUDEDIR = include

CFLAGS = -Wall -std=c++17 -O3 -I $(INCLUDEDIR)

all: clean $(BUILDDIR)/main

$(BUILDDIR)/data.o: $(SRCDIR)/decoder/data.cpp
	g++ $(CFLAGS) -c $(SRCDIR)/decoder/data.cpp -o $(BUILDDIR)/data.o

$(BUILDDIR)/decoder.o: $(SRCDIR)/decoder/decoder.cpp
	g++ $(CFLAGS) -c $(SRCDIR)/decoder/decoder.cpp -o $(BUILDDIR)/decoder.o

$(BUILDDIR)/decoder.a: $(BUILDDIR)/data.o $(BUILDDIR)/decoder.o
	ar rvs $(BUILDDIR)/decoder.a $(BUILDDIR)/data.o $(BUILDDIR)/decoder.o

$(BUILDDIR)/shape.o: $(SRCDIR)/shapes/shape.cpp
	g++ $(CFLAGS) -c $(SRCDIR)/shapes/shape.cpp -o $(BUILDDIR)/shape.o -I $(INCLUDEDIR)/shapes

$(BUILDDIR)/printer.o: $(SRCDIR)/printer.cpp
	g++ $(CFLAGS) -c $(SRCDIR)/printer.cpp -o $(BUILDDIR)/printer.o

$(BUILDDIR)/game.o: $(SRCDIR)/game.cpp
	g++ $(CFLAGS) -c $(SRCDIR)/game.cpp -o $(BUILDDIR)/game.o

$(BUILDDIR)/main: $(BUILDDIR)/game.o $(BUILDDIR)/decoder.a $(BUILDDIR)/shape.o $(BUILDDIR)/printer.o $(SRCDIR)/main.cpp
	g++ $(CFLAGS) $(SRCDIR)/main.cpp $(BUILDDIR)/game.o $(BUILDDIR)/decoder.a $(BUILDDIR)/shape.o $(BUILDDIR)/printer.o -o $(BUILDDIR)/main

run: all
	$(BUILDDIR)/main

clean:
	rm -rf $(BUILDDIR)/*