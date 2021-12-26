TARGET := SpaceCollider
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,${SOURCES})
CXXFLAGS += -Iinclude -std=c++20 -g
LDFLAGS += -lGL -lGLU -lglut -lfreeimage

all: ${TARGET}

${TARGET}: ${OBJECTS}
	${CXX} -o $@ $^ ${LDFLAGS}

clean:
	rm -f ${TARGET} ${OBJECTS}

.PHONY: clean