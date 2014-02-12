#	Knotscape
#	Makefile for GNU Make
#

# Edit the next two lines if necessary

CC = /usr/bin/gcc
CFLAGS = -O2 -s -fomit-frame-pointer

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
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(SRCDIR)/$@.c

glsig:
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(SRCDIR)/$@.c $(MATHFLAGS)
	
draw:
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(SRCDIR)/ken.c $(SRCDIR)/draw_main.c\
            $(SRCDIR)/triang.c $(SRCDIR)/nodeseq.c $(MATHFLAGS) -I$(INCDIR)
