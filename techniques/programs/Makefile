# tools
CC := gcc
RM := rm -f
CP := cp

# flags
CFLAGS := -m32 -ggdb -fno-stack-protector -mpreferred-stack-boundary=2
LDFLAGS := -m32 -L/usr/lib32
LDLIBS :=

# sources
tsources := program1.c program2.c program3.c program4.c program5.c program6.c program7.c
sources := $(tsources) tmalloc.c
targets := program1 program2 program3 program4 program5 program6 program7

# gmake magic
.PHONY: default all clean generate

#targets
default: all
all: $(targets)

generate:  
	./base/generatesrc.py

install: $(targets)
	-$(RM) /tmp/program?
	execstack -s program1
	execstack -s program2
	execstack -s program3
	execstack -s program4
	execstack -s program5
	$(CP) $(targets) /tmp


setuid:
ifeq ($(shell id -u),0)
	chown root:root /tmp/program?
	chmod 4755 /tmp/program?
	echo 0 > /proc/sys/kernel/randomize_va_space
else
	@echo "'make setuid' must be run as root -- use su or a root login!"
endif


clean:
	$(RM) $(targets) $(sources:.c=.o) $(tsources)


#dependencies
program4: tmalloc.o
tmalloc.o program4.o: tmalloc.h
