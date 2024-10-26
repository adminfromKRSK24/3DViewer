CC = g++
CFLAGS = -std=c++17 -Wall -Werror -Wextra -g -fsanitize=address -lm
VFLAGS = -Wall -Werror -Wextra -lm

CTEST = -lgtest --coverage
TESTS = ./tests/parcer_test.cpp



ifeq ($(UNAME), Linux)
	CTEST = -lgtest -pthread --coverage
	COVERAGE = xdg-open coverage_report.html
else
	CTEST = -lgtest --coverage
	COVERAGE = open coverage_report.html
endif

.PHONY: all build test install uninstall dvi dvi_clean dist gcov_report

all: clean test

install : 
	mkdir -p ./build
	cp -rf *.h *.cpp *.ui *.pro ./build
	cd ./build; qmake untitled.pro
	make -C ./build
	chmod -R u+wrx ./build
	cd ./build; rm -rf *.h *.cpp *.ui *.o *.pro Makefile

uninstall:
	rm -rf ./build

dvi:
	open ./docs/redmi.pdf

dist:
	mkdir dist
	cp -r ./build/* dist/
	tar cvzf 3dview_dist.tgz dist/
	rm -rf dist/
 #	распаковка 
 #	tar -xvf smartcalc_dist.tgz

test: clean
	$(CC) $(VFLAGS) ./tests/all_test.cpp $(TESTS) ./model.cpp -o test $(CTEST)
	./test

gcov_report: test
	gcovr -r . --html --html-details -o coverage_report.html
	$(COVERAGE)

clang:
	cp ../materials/linters/.clang-format ./
	clang-format -i *.cpp *.h
	clang-format -n *.cpp *.h
	rm -rf .clang-format

leaks: clean
	$(CC) $(VFLAGS) ./tests/all_test.cpp $(TESTS) ./model.cpp -o test $(CTEST)
	leaks -atExit -- ./test

clean:
	rm -f *.html *.o *.a *.gcno *.gcda *.info *.out *.css main test
	rm -rf *.dSYM
	rm -rf ./report
	rm -rf *.tgz
	clear
