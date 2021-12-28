TARGET := SpaceCollider
HEADERS := $(wildcard include/*.hpp)
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,${SOURCES})
CXXFLAGS := -Wall -Wextra -Wno-deprecated-copy -g -O0 -Iinclude -std=c++20 -g
LIBS := -lGL -lGLU -lglut -lfreeimage

all: ${TARGET}

${TARGET}: ${OBJECTS} ${HEADERS}
	${CXX} -o $@ ${OBJECTS} ${LIBS}

clean:
	rm -f ${TARGET} ${OBJECTS}

format:
	clang-format -i src/*.cpp include/*.hpp

.PHONY: clean format all