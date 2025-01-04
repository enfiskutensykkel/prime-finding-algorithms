CFLAGS += -Wall -Wextra -pedantic -Ilib

ifdef RELEASE
CFLAGS += -DNDEBUG=1 -O3
endif

EXECS := $(patsubst src/%.c,%,$(wildcard src/*.c))

.PHONY: all lib clean $(addsuffix -clean,$(EXECS))
all: $(EXECS)

define executable
$(1): src/$(1).o lib.a
	$$(CC) -o $$@ $$^

$(1)-clean:
	-$$(RM) $(1) src/$(1).o
endef
$(foreach target,$(EXECS),$(eval $(call executable,$(target))))

lib.a: lib/isqrt.o lib/parse.o
	$(AR) rcs $@ $^

clean: $(addsuffix -clean,$(EXECS))
	-$(RM) lib.a lib/*.o

lib/%.o: lib/%.c lib/%.h
	$(CC) -Wall -Ilib -c -o $@ $<

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
