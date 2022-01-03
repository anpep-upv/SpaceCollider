SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

CXXFLAGS := -Wall -Wextra -Wno-deprecated-copy -g -Iinclude -std=c++20
LIBS := -lGL -lGLU -lglut -lfreeimage

.PHONY: all clean

all: SpaceCollider

clean:
	${RM} ${OBJECTS} ${DEPENDS} SpaceCollider

# Linking the executable from the object files
SpaceCollider: ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

-include $(DEPENDS)

%.o: %.cpp Makefile
	${CXX} ${CXXFLAGS} -MMD -MP -c $< -o $@