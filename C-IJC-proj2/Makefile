CC = gcc
CFLAGS_SHARED= -std=c99 -Wall -Wextra -pedantic -fPIC -g -c
CFLAGS_STATIC= -std=c99 -Wall -Wextra -pedantic -g -c
CFLAGS= -std=c99 -Wall -Wextra -pedantic -g

LDFLAGS= -shared
RM= rm -f
TARGET_LIB=libhtab.so
TARGET_STATIC_LIB=libhtab.a
SRCS = htab_bucket_count.c htab_clear.c htab_find.c htab_foreach.c htab_free.c htab_init.c htab_lookup_add.c htab_move.c htab_remove.c htab_size.c
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB} ${TARGET_STATIC_LIB} tail tail2 wordcount wordcount-dynamic

#$(SRCS:.c=.d):%.d:%.c není vhodno pro tento případ protože se překládaji i veci s jinými CFLAGS
#	$(CC) $(CFLAGS) -MM $< | >$@
#include $(SRCS:.c=.d)

# dynamicka kinhovna
shared-htab_bucket_count.o: htab_bucket_count.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_clear.o: htab_clear.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_find.o: htab_find.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_foreach.o: htab_foreach.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_free.o: htab_free.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_init.o: htab_init.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_lookup_add.o: htab_lookup_add.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_move.o: htab_move.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_remove.o: htab_remove.c htab_lib.h
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-hash_function.o: hash_function.c
	$(CC) $(CFLAGS_SHARED) -o $@ $<
shared-htab_size.o: htab_size.c
	$(CC) $(CFLAGS_SHARED) -o $@ $<

$(TARGET_LIB): shared-htab_size.o shared-htab_bucket_count.o shared-htab_clear.o shared-htab_find.o shared-htab_foreach.o shared-htab_free.o shared-htab_init.o shared-htab_lookup_add.o shared-htab_move.o shared-htab_remove.o shared-hash_function.o
	$(CC) $(LDFLAGS) -o  $@ $^


#staticka knihovna
static-htab_bucket_count.o: htab_bucket_count.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_clear.o: htab_clear.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_find.o: htab_find.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_foreach.o: htab_foreach.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_free.o: htab_free.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_init.o: htab_init.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_lookup_add.o: htab_lookup_add.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_move.o: htab_move.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_remove.o: htab_remove.c htab_lib.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-hash_function.o: hash_function.c
	$(CC) $(CFLAGS_STATIC) -o $@ $<
static-htab_size.o: htab_size.c
	$(CC) $(CFLAGS_STATIC) -o $@ $<


$(TARGET_STATIC_LIB): static-htab_size.o static-hash_function.o static-htab_bucket_count.o static-htab_clear.o static-htab_find.o static-htab_foreach.o static-htab_free.o static-htab_init.o static-htab_lookup_add.o static-htab_move.o static-htab_remove.o
	ar crs libhtab.a $^
	ranlib libhtab.a
# make wordcount
io.o:io.c io.h
	$(CC) $(CFLAGS_STATIC) -o $@ $<

wordcount:  wordcount.c io.o ${TARGET_STATIC_LIB}
	$(CC) $(CFLAGS) wordcount.c libhtab.a io.o -o wordcount

wordcount-dynamic: wordcount.c io.o ${TARGET_LIB}
	$(CC) $(CFLAGS) -L. -lhtab -o wordcount-dynamic wordcount.c io.o

tail:tail.c
	$(CC) $(CFLAGS) -o $@ $^
tail2:tail2.cc
	g++ -std=c++11 -pedantic -Wall -o $@ $^

.PHONY: clean
clean:
	-${RM}  *.o tail tail2 wordcount wordcount-dynamic

.PHONY: pack
pack: all
	zip xjurca08.zip *.c *.cc *.h Makefile
