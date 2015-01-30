.PHONY: all
all:

CPPFLAGS := -Wall -Wextra -pedantic -Wno-variadic-macros
CPPFLAGS += -MD
CPPFLAGS += -pthread
CPPFLAGS += -I.
CXXFLAGS += -std=c++11

LDFLAGS := -pthread

ifndef V
Q := @
endif

include util/module.mk
include net/module.mk
include reactor/module.mk
include tests/module.mk
include coverage.mk

.PHONY: check
check: run_testUnits

CLEAN_DIRS := \
	out

.PHONY: clean
clean:
	$(if $Q,@echo "  CLEAN $(CLEAN_DIRS)")
	$Qrm -rf $(CLEAN_DIRS)
