#ALPHA VERSION
#edit:test.o gao.o
#	g++ -o edit test.o gao.o
#test.o:test.cpp gao.h
#	g++ -c test.cpp
#gao.o:gao.cpp gao.h
#	g++ -c gao.cpp
#.PHONY:clean
#clean:
#	rm edit test.o gao.o
#

#BETA VERSION
#objects=test.o gao.o gao2.o
#
#edit:$(objects)
#	g++ -o edit $(objects)
#teat.o:gao.h
#gao.o:gao.h 
#gao2.o:gao2.h
#
#.PHONY:clean
#clean:
#	rm edit $(objects)

CC=g++

objects=gao.o gao2.o
all:$(objects)
	$(CC) -o all $(objects)
%.o:%.cpp
	$(CC) -c $< -o $@

.PHONY:clean cdhome test
clean:
	rm *.o
cdhome:
	cd /home;pwd
test:
	@echo "this is a test"
