# compilation flags
CC = g++
CFLAGS = -c -std=c++17 -O2 -Wall

OBJDIR := bin
OBJS := $(addprefix $(OBJDIR)/, ipmt.o Huffman.o SuffixArray.o BinIO.o)

$(OBJDIR)/%.o : ./src/%.cpp
	$(CC) $(CFLAGS) $(OUTPUT_OPTION) $<

all: $(OBJDIR)/ipmt

$(OBJDIR)/ipmt: $(OBJS)
	$(CC) $(OBJS) $(OUTPUT_OPTION)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -r bin