TARGET := SpaceCollider
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,${SOURCES})
CXXFLAGS := -Wall -Wextra -g -O0 -Iinclude -std=c++20 -g
LIBS := -lGL -lGLU -lglut -lfreeimage

all: ${TARGET}

${TARGET}: ${OBJECTS}
	${CXX} -o $@ $< ${LIBS}

clean:
	rm -f ${TARGET} ${OBJECTS}

format:
	clang-format -i src/*.cpp include/*.hpp

.PHONY: clean format all