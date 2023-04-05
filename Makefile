#	Knotscape
#	Makefile for GNU Make
#

# Edit the next two lines if necessary

CC = gcc
CFLAGS = -O2 -fomit-frame-pointer

MATHFLAGS = -lm

SRCDIR = ./src
BINDIR = ./bin
INCDIR = ./src

VPATH = ./bin

all: knotfind mbt_ken orient poly1 poly2 horozoom\
     knot_group_reps checkknot draw braid2dt\
     dupsearch decode_a decode_n locate_a locate_n glsig draw2ps

knotfind mbt_ken orient poly1 poly2 knot_group_reps dupsearch draw2ps\
     checkknot horozoom braid2dt decode_a decode_n locate_a locate_n:
	@ mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(SRCDIR)/$@.c

glsig:
	@ mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(SRCDIR)/$@.c $(MATHFLAGS)
	
draw:
	@ mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(SRCDIR)/ken.c $(SRCDIR)/draw_main.c\
            $(SRCDIR)/triang.c $(SRCDIR)/nodeseq.c $(MATHFLAGS) -I$(INCDIR)

clean:
	rm -rf $(BINDIR)
