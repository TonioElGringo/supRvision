all: scan
scan: scan.cpp
	g++ -O2 -Wall -o scan scan.cpp `pkg-config --cflags --libs opencv`
clean:
	rm -f scan
