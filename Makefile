CC=g++
PLUGIN = voncount.so
OBJS = voncount.o
PLUGINS_DIR = $(shell $(CC) -print-file-name=plugin)
CFLAGS += -I$(PLUGINS_DIR)/include -fPIC -O0 -g3

# I use this for debugging.  That path contains a version of gcc with debug
# symbols and no optimization.
# GCCPATH=~/proj/gcc-4.8.0-obj/

$(PLUGIN): $(OBJS)
	$(CC) -shared $^ -o $@

test: $(PLUGIN) test.c test2.c
	$(CC) -fplugin=./$(PLUGIN) test.c test2.c -o $@

debug: $(DBG_PLUGIN)
	exec gdb --args $(GCCPATH)/gcc/cc1 \
        -fplugin=./$(PLUGIN) test.c -I$(GCCPATH)/gcc/include

clean:
	$(RM) $(OBJS) $(PLUGIN) test test2.o
