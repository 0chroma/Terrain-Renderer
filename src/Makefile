CC = g++
CFLAGS = -g #-Wall -g -Wl,-rpath,'./lib' #'${ORIGIN}/lib'
PROG = renderer

SRCS = main.cpp Image.cpp QuadTree.cpp QuadTreeNode.cpp

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT -framework GLU -framework png
else
    LIBS = -lglut -lGLU -lGL -lpng
endif

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
