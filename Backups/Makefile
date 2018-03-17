
DEBUG= 
OS = $(shell uname)
ifeq ($(OS),Linux)
  CC=g++ -Wall 
endif
ifeq ($(OS),Darwin)
  CC=clang++ -stdlib=libc++
endif

CFLAGS=-c -std=c++0x -O2 
TARGET_LIB=parambin
CURRENT_VERSION=1.1
COMPAT_VERSION=1.1
LIB_FILE=lib${TARGET_LIB}.${CURRENT_VERSION}.dylib

SOURCES=$(shell find src -type f -name *.cpp)
OBJECTS=$(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))
PARAM_TEST=bin/run


all: dylib test

dylib: ${OBJECTS}
	${CC} -dynamiclib \
		-current_version ${CURRENT_VERSION} \
		-compatibility_version ${COMPAT_VERSION} \
		${OBJECTS} -o ${LIB_FILE}

obj/%.o: src/%.cpp
	${CC} ${CFLAGS} -I include $< -o $@

test: ${PARAM_TEST}

${PARAM_TEST}: ${OBJECTS}
	${CC} ${DEBUG} ${OBJECTS} -o ${PARAM_TEST} 


install: dylib
	./bin/install.py ${TARGET_LIB} ${CURRENT_VERSION}
	rm -f *.dylib

uninstall:
	./bin/uninstall.py ${TARGET_LIB} ${CURRENT_VERSION}

clean:
	rm -f ${OBJECTS} ${LIB_FILE}
		





