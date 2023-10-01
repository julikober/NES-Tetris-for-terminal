BUILDDIR = build
SRCDIR = src
INCLUDEDIR = include

CFLAGS = -Wall -std=c++17

all: clean $(BUILDDIR)/game

$(BUILDDIR)/data.o: $(SRCDIR)/decoder/data.cpp
	g++ $(CFLAGS) -c $(SRCDIR)/decoder/data.cpp -o $(BUILDDIR)/data.o -fpic -I $(INCLUDEDIR)/decoder

$(BUILDDIR)/decoder.o: $(SRCDIR)/decoder/decoder.cpp
	g++ $(CFLAGS) -c $(SRCDIR)/decoder/decoder.cpp -o $(BUILDDIR)/decoder.o -fpic -I $(INCLUDEDIR)/decoder

$(BUILDDIR)/decoder.so: $(BUILDDIR)/data.o $(BUILDDIR)/decoder.o
	g++ -shared -o $(BUILDDIR)/decoder.so $(BUILDDIR)/data.o $(BUILDDIR)/decoder.o 

$(BUILDDIR)/game: $(SRCDIR)/game.cpp $(BUILDDIR)/decoder.so
	g++ $(CFLAGS) $(SRCDIR)/game.cpp -o $(BUILDDIR)/game $(BUILDDIR)/decoder.so -I $(INCLUDEDIR)/decoder

run: all
	$(BUILDDIR)/game

clean:
	rm -rf $(BUILDDIR)/*