CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local -lmpsse

all: i2c_scanner

i2c_scanner: i2c_scanner.c
	$(CC) $(CFLAGS) i2c_scanner.c -o i2c_scanner $(LDFLAGS)

clean:
	rm -f i2c_scanner
