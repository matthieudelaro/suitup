GTEST_DIR=../../googletest-master/googletest

test: test.c++ utils.cpp
	g++ -isystem ${GTEST_DIR}/include -pthread test.c++ utils.cpp libgtest.a -o unittest
	./unittest


