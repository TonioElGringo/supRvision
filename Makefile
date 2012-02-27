all: scan camtest
scan: scan.cpp
	g++ -O2 -Wall -o scan scan.cpp `pkg-config --cflags --libs opencv`

camtest: camtest.cpp
	g++ -O2 -Wall -o camtest camtest.cpp `pkg-config --cflags --libs opencv`
clean:
	rm -f scan
