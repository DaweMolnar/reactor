# tests/func/module.mk

check: out/testFuncs

testFuncs_SOURCES := \
	tests/func/client.cc \
	tests/func/main.cc

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
