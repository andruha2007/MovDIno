CFLAGS = -Wall -Wextra -I.
SRCS = movdino.c \
       core/interpreter.c \
       core/executor.c \
       data/field.c \
       parser/parser.c \
       parser/specific_parser.c \
       utils/errors.c \
       utils/validator.c
OBJS = $(SRCS:.c=.o)
TARGET = movdino

$(TARGET): $(OBJS)
	gcc $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean