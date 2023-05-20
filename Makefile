all: test_pqueue test_bstream zap unzap

test_pqueue: test_pqueue.cc pqueue.h
	g++ -g -Wall -Werror -o $@ $< -std=c++11 -pthread -lgtest

test_bstream: test_bstream.cc bstream.h
	g++ -Wall -Werror -o $@ $< -std=c++11 -pthread -lgtest

zap: zap.cpp huffman.h
	g++ -Wall -Werror -o $@ $< -std=c++11

unzap: unzap.cpp huffman.h
	g++ -Wall -Werror -o $@ $< -std=c++11

clean:
	-rm -f test_pqueue test_bstream zap unzap *.dat
