NAME=lz4demo
CCFLAGS=-n -s -nostartfiles -nostdlib

all: $(NAME).c lz4.c
	@x86_64-nacl-gcc -o $(NAME).nexe $(CCFLAGS) -Wall -msse4.1 \
	-O2 -I$(ZEROVM_ROOT) -I$(ZEROVM_ROOT)/tests/functional $^ \
	$(ZEROVM_ROOT)/tests/functional/include/libzvmlib.a
	@dd if=/dev/zero of=input.data bs=1048576 count=256 2> /dev/null
	@sed 's#PWD#$(PWD)#g' $(NAME).template > $(NAME).manifest
	@$(ZEROVM_ROOT)/zerovm -PQs $(NAME).manifest
	@./lz4demo -t output.lz4

clean:
	rm -f $(NAME).nexe $(NAME).o lz4.o *.log *.lz4 *.data *.manifest
