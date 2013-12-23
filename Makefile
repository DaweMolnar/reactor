TARGETS := out/client

ifndef V
Q := @
endif

.PHONY: all
all: $(TARGETS)

client_TESTABLE_SOURCES := \
	src/ErrnoException.cc \
	src/Fd.cc \
	src/Host.cc \
	src/Ip.cc \
	src/Service.cc \
	src/Socket.cc \
	src/DiffTime.cc \
	src/Time.cc \
	src/Timer.cc \
	src/ActionsGuard.cc \
	src/Timers.cc \
	src/PollDemuxer.cc \
	src/Dispatcher.cc \
	src/Client.cc

client_SOURCES := \
	$(client_TESTABLE_SOURCES) \
	src/client.cc

testUnits_SOURCES := \
	$(client_TESTABLE_SOURCES) \
	tests/unit/mock/PrimitiveByFormatFactory.cc \
	tests/unit/mock/MockRegistry.cc \
	tests/unit/mock/Mocked.cc \
	tests/unit/mock/unistd.cc \
	tests/unit/mock/time.cc \
	tests/unit/ErrnoTester.cc \
	tests/unit/FdTester.cc \
	tests/unit/DiffTimeTester.cc \
	tests/unit/TimeTester.cc \
	tests/unit/testUnits.cc

CPPFLAGS := -Wall -Wextra -pedantic -Wno-variadic-macros
CPPFLAGS += -MD


testUnits_OBJECTS := $(sort $(addprefix out/testUnits.d/,$(addsuffix .o,$(basename $(testUnits_SOURCES)))))
-include $(addsuffix .d,$(basename $(testUnits_OBJECTS)))

out/testUnits.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

testUnits_WRAPPED_SYMBOLS := \
	close \
	read \
	write \
	fcntl \
	gettimeofday

LINKER_FLAG := -Wl,

out/testUnits: CPPFLAGS += -I.
out/testUnits: LDFLAGS += $(addprefix $(LINKER_FLAG)--wrap=,$(testUnits_WRAPPED_SYMBOLS))
out/testUnits: LDLIBS += $(shell cppunit-config --libs)

ifeq "$(CXX)" "g++"
out/testUnits: CPPFLAGS += -g -O0 -coverage
out/testUnits: LDFLAGS += -coverage
endif

out/testUnits: $(testUnits_OBJECTS)
	$(if $Q,@echo "  LINK  $@")
	$Q$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)


client_OBJECTS := $(sort $(addprefix out/client.d/,$(addsuffix .o,$(basename $(client_SOURCES)))))
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
