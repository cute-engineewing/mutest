CC	?= gcc
RM	= rm -f
AR	= ar

CFLAGS	+= -std=c99 -pedantic -Wall -Werror -Wextra -fpic -Iinc -O0
LDFLAGS	+=

test: CFLAGS	+=  -Iinc --coverage
test: LDFLAGS	+= --coverage
test: CC	= gcc

TARGET	= libmutest

SRCS	= mutest.c

OBJS	= $(addprefix src/, $(SRCS:.c=.o))

TEST_SRCS	= test.c test_result.c test_expect.c
TEST_OBJS	= $(addprefix tests/, $(TEST_SRCS:.c=.o))

all: $(TARGET).a $(TARGET).so

$(TARGET).so: $(OBJS)
	$(CC) -o $@ $^ -shared $(LDFLAGS)

$(TARGET).a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

test: $(TARGET).a $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS) $(LDFLAGS) -L. -l:libmutest.a
	@./$@

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS:.o=.gcda)
	$(RM) $(OBJS:.o=.gcno)
	$(RM) $(TEST_OBJS:.o=.gcda)
	$(RM) $(TEST_OBJS:.o=.gcno)
	$(RM) $(TEST_OBJS)
	$(RM) $(TARGET).a $(TARGET).so

re: clean all

.PHONY: all clean clean re test
