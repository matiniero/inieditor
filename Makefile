CC = gcc
CFLAGS  = -ansi -pedantic

default: editor

editor:  editor.o iniedit.o
	$(CC) $(CFLAGS) -o editor editor.o iniedit.o

editor.o:  editor.c
	$(CC) $(CFLAGS) -c editor.c

iniedit.o:  iniedit.c
	$(CC) $(CFLAGS) -c iniedit.c

clean: 
	$(RM) editor *.o *~ 
