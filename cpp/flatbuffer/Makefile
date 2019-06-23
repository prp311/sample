CC = g++

OUT=out

CFLAGS = -Wall -g -std=c++11

INCLUDES = -I$(HOME)/thirdparty/flatbuffers/include -I./ -I$(OUT)

LFLAGS = -L$(HOME)/thirdparty/flatbuffers/

LIBS = -lflatbuffers

SRCS = main.cpp

OBJS = $(addprefix $(OUT)/,$(SRCS:.cpp=.o))

MAIN = $(OUT)/main

FLATC=$(HOME)/thirdparty/flatbuffers/flatc

GENHEADER=$(OUT)/fbmessage_generated.h

.PHONY: depend clean all

all: $(OUT) $(MAIN) 

$(GENHEADER): fbmessage.fbs
	$(FLATC) -o $(OUT) -c fbmessage.fbs

$(MAIN): $(GENHEADER) $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(OUT)/%.o: %.cpp
	$(CC) -g $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(GENHEADER) $(MAIN)
	$(RM) -rf $(OUT)

$(OUT):
	(mkdir $(OUT))

depend: $(SRCS) $(OUT)
	makedepend $(INCLUDES) $^

