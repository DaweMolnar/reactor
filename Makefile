TARGETS := client

.PHONY: all
all: $(TARGETS)

client_SOURCES := \
	ErrnoException.cc \
	client.cc

client_OBJECTS := $(addsuffix .o,$(basename $(client_SOURCES)))

client: LDFLAGS += -lstdc++
client: $(client_OBJECTS)

CPPFLAGS := -Wall -Wextra -pedantic
#-Weffc++

.PHONY: clean
clean:
	rm -f $(TARGETS) $(client_OBJECTS)
