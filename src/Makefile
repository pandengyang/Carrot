CC = gcc
CPPFLAGS = -I./include -I../include
CFLAGS = 
LDFLAGS =
LIBS= -pthread ../queue/libqueue.a

RM = rm -rf
MKDIR = mkdir -p

DEPS = .dep

TARGET = test 

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET:$(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(LDFLAGS)

$(OBJS):%.o:%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

sinclude $(patsubst %.c, $(DEPS)/%.d, $(SRCS))
$(DEPS)/%.d:%.c
	$(MKDIR) $(DEPS); \
	rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[: ]*,\1.o $@: ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$

.PHONY:clean cleanall

clean:
	$(RM) $(TARGET) $(OBJS)

cleanall:
	$(RM) $(TARGET) $(OBJS) $(DEPS) .*.sw?
