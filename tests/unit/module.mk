# tests/unit/module.mk

testUnits_SOURCES := \
	$(libreactor_SOURCES) \
	tests/unit/mock/PrimitiveByFormatFactory.cc \
	tests/unit/mock/MockRegistry.cc \
	tests/unit/mock/Mocked.cc \
	tests/unit/mock/unistd.cc \
	tests/unit/mock/time.cc \
	tests/unit/SpecifierTester.cc \
	tests/unit/TimerTester.cc \
	tests/unit/TimersTester.cc \
	tests/unit/DispatcherTester.cc \
	tests/unit/ThreadTester.cc \
	tests/unit/ThreadPoolTester.cc \
	tests/unit/ThreadMutexTester.cc \
	tests/unit/ThreadConditionTester.cc \
	tests/unit/testUnits.cc

testUnits_SOURCES += \
	$(libutil_SOURCES) \
	tests/unit/ErrnoTester.cc \
	tests/unit/FdTester.cc \
	tests/unit/DiffTimeTester.cc \
	tests/unit/TimeTester.cc \
	tests/unit/AutoFdTester.cc

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

out/testUnits: LDFLAGS += $(addprefix $(LINKER_FLAG)--wrap=,$(testUnits_WRAPPED_SYMBOLS))
out/testUnits: LDLIBS += $(shell cppunit-config --libs)

ifeq "$(CXX)" "g++"
out/testUnits: CPPFLAGS += -g -O0 -coverage
out/testUnits: LDFLAGS += -coverage
endif

out/testUnits: $(testUnits_OBJECTS)
	$(if $Q,@echo "  LINK  $@")
	$Q$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

.PHONY: run_testUnits
run_testUnits: out/testUnits
	$Qfind $<.d/ -name '*.gcda' -delete
	$(if $Q,@echo "  RUN   $<")
	$Q$<
