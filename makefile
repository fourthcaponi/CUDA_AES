OS=$(shell uname)

all:
	g++ -o aes -lm -lGL -lglut -lGLU -g -Wall main.cpp 


