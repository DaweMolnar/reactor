# client.mk

all: out/client

client_SOURCES := \
	src/client.cc

client_OBJECTS := $(sort $(addprefix out/client.d/,$(addsuffix .o,$(basename $(client_SOURCES)))))
-include $(addsuffix .d,$(basename $(client_OBJECTS)))

out/client: CPPFLAGS += -g -O0

out/client.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/client: out/libreactor.a
out/client: out/libutil.a
out/client: LDLIBS += -lreactor -lutil
out/client: LDFLAGS += -Lout/

out/client: $(client_OBJECTS)
	$(if $Q,@echo "  LINK  $@")
	$Q$(CXX) $(LDFLAGS) $(filter-out %.a,$^) $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)
