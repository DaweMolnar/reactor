# tests/func/module.mk

check: run_funcTests

.PHONY: run_funcTests
run_funcTests: tests/func/script/run-tests.sh out/testFuncs
	$(if $Q,@echo "  RUN   $<")
	$Q$< $(FUNCTIONAL_TESTS)

testFuncs_TESTER_SOURCES := \
	tests/func/ClientTester.cc

FUNCTIONAL_TESTS := $(notdir $(basename $(testFuncs_TESTER_SOURCES)))

testFuncs_SOURCES := \
	$(testFuncs_TESTER_SOURCES) \
	tests/func/testFuncs.cc

testFuncs_OBJECTS := $(sort $(addprefix out/testFuncs.d/,$(addsuffix .o,$(basename $(testFuncs_SOURCES)))))
-include $(addsuffix .d,$(basename $(testFuncs_OBJECTS)))

out/testFuncs: CPPFLAGS += -g -O0

out/testFuncs.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/testFuncs: out/libreactor.a
out/testFuncs: out/libutil.a
out/testFuncs: out/libthread.a
out/testFuncs: out/libnet.a
out/testFuncs: LDLIBS += -lreactor -lutil -lthread -lnet
out/testFuncs: LDFLAGS += -Lout/

out/testFuncs: $(testFuncs_OBJECTS)
	$(if $Q,@echo "  LINK  $@")
	$Q$(CXX) $(LDFLAGS) $(filter-out %.a,$^) $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)
