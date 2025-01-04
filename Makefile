CFLAGS += -Wall -Wextra -pedantic -Ilib

ifdef RELEASE
CFLAGS += -DNDEBUG=1 -O3
endif

EXECS := dijkstra sieve trial trial_list b001 b001_array

.PHONY: all lib clean $(addsuffix -clean,$(EXECS))
all: $(EXECS)

define executable
$(1): $(1).o lib.a
	$$(CC) -o $$@ $$^

$(1)-clean:
	-$$(RM) $(1) $(1).o
endef
$(foreach target,$(EXECS),$(eval $(call executable,$(target))))

lib.a: parse.o isqrt.o
	$(AR) rcs $@ $^

clean: $(addsuffix -clean,$(EXECS))
	-$(RM) lib.a parse.o isqrt.o

%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
