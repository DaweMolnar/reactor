TARGETS := out/client

ifndef V
Q := @
endif

.PHONY: all
all: $(TARGETS)

include libreactor.mk
include testunits.mk
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
