CC=gcc
CFLAGS=-Wall -Wextra -lm -g
SRC=src/main.c src/kdtree.c src/csv_reader.c src/fuzzy_cmeans.c src/dbscan.c
OBJ=$(SRC:.c=.o)
TARGET=robot_spatial

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET) data/lidar.csv -kd_nearest 0,0
	