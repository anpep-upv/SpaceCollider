SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

CXXFLAGS := -Wall -Wextra -Wno-deprecated-copy -Wno-unused-parameter -g -Iinclude -std=c++20

ifeq ($(shell uname -s),Darwin)
CXXFLAGS += -I/opt/homebrew/include -DGL_SILENCE_DEPRECATION
LIBS += -framework OpenGL -framework GLUT -L/opt/homebrew/lib 
else
LIBS += -lGL -lGLU -lglut -lpulse-simple -lpulse -pthread
endif

LIBS += -lfreeimage


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