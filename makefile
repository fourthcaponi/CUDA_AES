OS=$(shell uname)

all:
<<<<<<< HEAD
	g++ -o aes -lm -g main.cpp 
=======
	g++ -o aes -lm -g -Wall main.cpp ByteSub.cpp KeyAdd.cpp MixColumn.cpp ShiftRow.cpp
>>>>>>> 37380d7d5d8b8e7d4eaeb8e3822b8298b683e875


