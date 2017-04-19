OS=$(shell uname)

all:
	g++ -o aes -lm -g -Wall main.cpp ByteSub.cpp KeyAdd.cpp MixColumn.cpp ShiftRow.cpp


