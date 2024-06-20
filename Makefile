CC=gcc

ep1: driverEP1.o EP1.o
	$(CC) -o ep1 EP1.o driverEP1.o