
INCLUDE = -I../log/  -I../base/ 
SOURCE  = ../log/*.cpp   ../base/*.cpp  *.cpp
LIB = -std=c++11 -ldl -lpthread


all:
	g++   -fpermissive -g -o main     $(INCLUDE) $(SOURCE) $(LIB)
clean:
	rm -rf *.o	main
