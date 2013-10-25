TARGETS := client

CPPFLAGS := -Wall -Wextra -pedantic

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(TARGETS)
