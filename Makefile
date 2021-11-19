all: compile link

compile:
	g++ -I src/include -c gui.cpp
link:
	g++ gui.o -o gui -L src/lib -l sfml-graphics -l sfml-window -l sfml-system