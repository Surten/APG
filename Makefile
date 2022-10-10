INCLUDE = -I/usr/include -I/usr/X11R6/include -I. -I./sgl-cpp/include
LIBDIR = -L. -L/usr/X11R6/lib -L/usr/lib -L/usr/lib64

COMPILERFLAGS = -Wall -O2 -DUSE_GUI=0 -std=c++17 -pthread -march=westmere
#COMPILERFLAGS = -Wall -g -O0 -DUSE_GUI=0 -std=c++17 -pthread -march=westmere
CC = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE) -DTEST$(TASK)
LFLAGS = -pthread

#LIBRARIES = -lglut -lGL -lGLU  #-lXext -lX11 -lm
LIBRARIES = -lm
SOURCES := testapp.cpp nffread.cpp timer.cpp hdrloader.cpp $(wildcard sgl-cpp/src/*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

NFFLIBRARIES = -lglut -lGL -lGLU  #-lXext -lX11 -lm
NFFSOURCES := nffview.cpp nffread.cpp hdrloader.cpp
NFFOBJS := $(patsubst %.cpp,%.o,$(NFFSOURCES))

default:
	make $(TASKNAME)

testapp: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $(LIBDIR) $(OBJS) $(LIBRARIES)

nffview: $(NFFOBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $(LIBDIR) $(NFFOBJS) $(NFFLIBRARIES)

.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

clean:
	rm -f testapp $(OBJS)

test0:	clean
	date
	make testapp TASK=0
	./testapp
	@convert results/test0a.tga results/test0a.png
	@convert results/test0b.tga results/test0b.png
	@composite results/test0a.tga results/test0a-ref.png -compose difference results/diff0a.png
	@composite results/test0b.tga results/test0b-ref.png -compose difference results/diff0b.png
#	valgrind --show-leak-kinds=all --leak-check=full ./testapp > /dev/null

test1:	clean
	date
	make testapp TASK=1
	./testapp
	@convert results/test1a.tga results/test1a.png
	@convert results/test1b.tga results/test1b.png
	@convert results/test1c.tga results/test1c.png
	@composite results/test1a.tga results/test1a-ref.png -compose difference results/diff1a.png
	@composite results/test1b.tga results/test1b-ref.png -compose difference results/diff1b.png
	@composite results/test1c.tga results/test1c-ref.png -compose difference results/diff1c.png
#	valgrind --show-leak-kinds=all --leak-check=full ./testapp > /dev/null

test2:	clean
	date
	make testapp TASK=2
	./testapp
	@convert results/test2a.tga results/test2a.png
	@convert results/test2b.tga results/test2b.png
	@convert results/test2c.tga results/test2c.png
	@convert results/test2d.tga results/test2d.png
	@composite results/test2a.tga results/test2a-ref.png -compose difference results/diff2a.png
	@composite results/test2b.tga results/test2b-ref.png -compose difference results/diff2b.png
	@composite results/test2c.tga results/test2c-ref.png -compose difference results/diff2c.png
	@composite results/test2d.tga results/test2d-ref.png -compose difference results/diff2d.png
#	valgrind --show-leak-kinds=all --leak-check=full ./testapp > /dev/null

test3:	clean
	date
	make testapp TASK=3
	./testapp
	@convert results/test3a.tga results/test3a.png
	@convert results/test3b.tga results/test3b.png
	@convert results/test3c.tga results/test3c.png
	@composite results/test3a.tga results/test3a-ref.png -compose difference results/diff3a.png
	@composite results/test3b.tga results/test3b-ref.png -compose difference results/diff3b.png
	@composite results/test3c.tga results/test3c-ref.png -compose difference results/diff3c.png
#	valgrind --show-leak-kinds=all --leak-check=full ./testapp > /dev/null

test4:	clean
	date
	make testapp TASK=4
	./testapp
	@convert results/test4a.tga results/test4a.png
	@convert results/test4b.tga results/test4b.png
	@convert results/test4c.tga results/test4c.png
	@composite results/test4a.tga results/test4a-ref.png -compose difference results/diff4a.png
	@composite results/test4b.tga results/test4b-ref.png -compose difference results/diff4b.png
	@composite results/test4c.tga results/test4c-ref.png -compose difference results/diff4c.png
#	valgrind --show-leak-kinds=all --leak-check=full ./testapp > /dev/null

test5:	clean
	date
	make testapp TASK=5
	./testapp
	@convert results/test5a.tga results/test5a.png
	@convert results/test5b.tga results/test5b.png
	@convert results/test5c.tga results/test5c.png
	@convert results/test5d.tga results/test5d.png
	@composite results/test5a.tga results/test5a-ref.png -compose difference results/diff5a.png
	@composite results/test5b.tga results/test5b-ref.png -compose difference results/diff5b.png
	@composite results/test5c.tga results/test5c-ref.png -compose difference results/diff5c.png
	@composite results/test5d.tga results/test5d-ref.png -compose difference results/diff5d.png
#	valgrind --show-leak-kinds=all --leak-check=full ./testapp > /dev/null
