TARGETS := client

CPPFLAGS := -Wall -Wextra -pedantic
#-Weffc++

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(TARGETS)
