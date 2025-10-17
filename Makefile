COMMON_SRC = main.c input.c ui_tree.c timing.c
LIB_SRC = tree.c
HEADERS = $(wildcard *.h)

COMMON_OBJ = $(COMMON_SRC:.c=.o)
LIB_OBJ = $(LIB_SRC:.c=.o)

TARGET = lab4b
LIB = libtree.a

all: $(TARGET)

$(TARGET): $(COMMON_OBJ) $(LIB)
	gcc -o $@ $(COMMON_OBJ) -L. -ltree -lgvc -lcgraph -lreadline

$(LIB): $(LIB_OBJ)
	ar rcs $@ $^

%.o: %.c $(HEADERS) timing.h
	gcc -c $< -o $@

clean:
	rm -f $(TARGET) $(LIB) $(COMMON_OBJ) $(LIB_OBJ)
