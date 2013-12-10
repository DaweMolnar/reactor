TARGETS := out/client

ifndef V
Q := @
endif

.PHONY: all
all: $(TARGETS)

client_TESTABLE_SOURCES := \
	src/ErrnoException.cc \
	src/Fd.cc

client_SOURCES := \
	$(client_TESTABLE_SOURCES) \
	src/client.cc

testUnits_SOURCES := \
	$(client_TESTABLE_SOURCES) \
	tests/unit/ErrnoTester.cc \
	tests/unit/testUnits.cc

CPPFLAGS := -Wall -Wextra -pedantic -Wno-variadic-macros
CPPFLAGS += -MD


testUnits_OBJECTS := $(addprefix out/testUnits.d/,$(addsuffix .o,$(basename $(testUnits_SOURCES))))
-include $(addsuffix .d,$(basename $(testUnits_OBJECTS)))

out/testUnits.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/testUnits: LDLIBS += $(shell cppunit-config --libs)
out/testUnits: CPPFLAGS += -I.

ifeq "$(CXX)" "g++"
out/testUnits: CPPFLAGS += -g -O0 -coverage
out/testUnits: LDFLAGS += -coverage
endif

out/testUnits: $(testUnits_OBJECTS)
	$(if $Q,@echo "  LINK  $@")
	$Q$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)


client_OBJECTS := $(addprefix out/client.d/,$(addsuffix .o,$(basename $(client_SOURCES))))
-include $(addsuffix .d,$(basename $(client_OBJECTS)))

out/client.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/client: $(client_OBJECTS)
	$(if $Q,@echo "  LINK  $@")
	$Q$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)


.PHONY: check
check: run_testUnits

.PHONY: run_testUnits
run_testUnits: out/testUnits
	$(if $Q,@echo "  RUN   $^")
	$Q$<

.PHONY: coverage
coverage: out/testUnits.cov.d

out/testUnits.cov.d: out/testUnits.cov
	$(if $Q,@echo "  GEN   $@")
	$Qgenhtml \
		$< \
		--output-directory $@ \
		--quiet

out/testUnits.cov: run_testUnits
	$(if $Q,@echo "  LCOV  $@")
	$Qlcov \
		--output-file $@ \
		--no-external \
		--capture \
		--directory out \
		--base-directory $(abspath .) \
		--quiet

CLEAN_DIRS := \
	out

.PHONY: clean
clean:
	$(if $Q,@echo "  CLEAN $(CLEAN_DIRS)")
	$Qrm -rf $(CLEAN_DIRS)
