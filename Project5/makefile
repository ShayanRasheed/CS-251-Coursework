build:
	rm -f tests.exe
	g++ tests.cpp -o tests.exe -lgtest -lgtest_main -lpthread -g -Wall
	
run:
	./tests.exe

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./tests.exe