.PHONY: all
all:

CPPFLAGS := -Wall -Wextra -pedantic -Wno-variadic-macros
CPPFLAGS += -MD
CPPFLAGS += -pthread
CPPFLAGS += -I.

LDFLAGS := -pthread

ifndef V
Q := @
endif

include libreactor.mk
include util/module.mk
include tests/unit/module.mk
include client.mk
include coverage.mk

.PHONY: check
check: run_testUnits

CLEAN_DIRS := \
	out

.PHONY: clean
clean:
	$(if $Q,@echo "  CLEAN $(CLEAN_DIRS)")
	$Qrm -rf $(CLEAN_DIRS)
