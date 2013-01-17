######################################################
#config
######################################################
AR = ar
CC = gcc
CFLAGS = -O4 -Wall --ansi
LDFLAGS =
LIBS = 
DEFS = -DDEBUG
HEADERS = ${HOME}/prg/inc
INCLUDES = -I$(HEADERS)
CREATE_LIB = memdbg.a
COMPILE = $(CC) $(LDFLAG) $(DEFS) $(INCLUDES)

#make target
SRCDIR = .
OBJS = \
		mti.o \
		mtil.o \
		msl.o


######################################################
#make
######################################################

test: $(CREATE_LIB)
	$(CC) -o libtest libtest.c $(CREATE_LIB) -lm

$(CREATE_LIB): $(OBJS)
	$(AR) -r $@ $(OBJS)

mti.o: $(SRCDIR)/mti.c
	$(COMPILE) $(CFLAGS) -c $(SRCDIR)/$*.c
mtil.o: $(SRCDIR)/mtil.c
	$(COMPILE) $(CFLAGS) -c $(SRCDIR)/$*.c
msl.o: $(SRCDIR)/msl.c
	$(COMPILE) $(CFLAGS) -c $(SRCDIR)/$*.c


install: $(INST_LIB) $(INST_INC)
#	cp $(INST_LIB) $(INST_LIB_DIR)
#	mkdir $(INST_INC_DIR)
#	cp $(INST_INC) $(INST_INC_DIR)

uninstall: 
#	rm $(INST_INC_DIR)/*.h
#	rmdir $(INST_INC_DIR)
#	rm $(INST_LIB_DIR)/$(INST_LIB)

clean:
	-rm $(OBJS) $(CREATE_LIB) libtest


