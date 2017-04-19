
DEBUG= 
OS = $(shell uname)
ifeq ($(OS),Linux)
  CC=g++ -Wall 
endif
ifeq ($(OS),Darwin)
  CC=clang++ -stdlib=libc++ -Wall
endif

CFLAGS=-c -std=c++11  
LDFLAGS=  -L/usr/local/lib -lpwutils
INCLUDES= -I include -I$/usr/local/include  

TARGET_LIB=parambin
CURRENT_VERSION=1.2
COMPAT_VERSION=1.2
LIB_FILE=lib${TARGET_LIB}.${CURRENT_VERSION}.dylib

SOURCES=$(shell find src -type f -name *.cpp)
OBJECTS=$(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))
TESTSRC=$(shell find test -type f -name *.cpp)
TESTOBJ=$(addprefix test/,$(notdir $(TESTSRC:.cpp=.o)))

RUNNABLE=bin/run
TEST1=test/test1
TEST2=test/test2
TEST3=test/test3
TEST4=test/test4


all: dylib run

tests: test1 test2 test3 test4

dylib: ${OBJECTS}
	${CC} -dynamiclib \
		-current_version ${CURRENT_VERSION} \
		-compatibility_version ${COMPAT_VERSION} \
		${LDFLAGS} ${OBJECTS} -o ${LIB_FILE}

obj/%.o: src/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} $< -o $@

test/%.o: test/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} $< -o $@

run: ${RUNNABLE}

${RUNNABLE}: ${OBJECTS} test/main.o
	${CC} ${DEBUG} ${LDFLAGS} ${OBJECTS} test/main.o -o ${RUNNABLE} 

test1: ${TEST1}

${TEST1}: ${OBJECTS} test/test1.o 
	${CC} ${DEBUG} ${LDFLAGS} ${OBJECTS} test/test1.o -o ${TEST1} 

test2: ${TEST2}

${TEST2}: ${OBJECTS} test/test2.o 
	${CC} ${DEBUG} ${LDFLAGS} ${OBJECTS} test/test2.o -o ${TEST2} 

test3: ${TEST3}

${TEST3}: ${OBJECTS} test/test3.o 
	${CC} ${DEBUG} ${LDFLAGS} ${OBJECTS} test/test3.o -o ${TEST3} 

test4: ${TEST4}

${TEST4}: ${OBJECTS} test/test4.o 
	${CC} ${DEBUG} ${LDFLAGS} ${OBJECTS} test/test4.o -o ${TEST4} 


install: dylib
	./bin/install.py ${TARGET_LIB} ${CURRENT_VERSION}
	rm -f *.dylib

uninstall:
	./bin/uninstall.py ${TARGET_LIB} ${CURRENT_VERSION}

clean:
	rm -f ${OBJECTS} 
		
cleanall:
	rm -f ${OBJECTS} ${LIB_FILE} ${TESTOBJ}





