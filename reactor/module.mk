# reactor/module.mk

all: out/libreactor.a

libreactor_SOURCE_NAMES := \
	Backlog.cc \
	Client.cc \
	Dispatcher.cc \
	PollDemuxer.cc \
	Reactor.cc \
	Socket.cc \
	Timer.cc \
	Timers.cc

libreactor_SOURCES := $(addprefix reactor/,$(libreactor_SOURCE_NAMES))
libreactor_OBJECTS := $(sort $(addprefix out/libreactor.a.d/,$(addsuffix .o,$(basename $(libreactor_SOURCES)))))
-include $(addsuffix .d,$(basename $(libreactor_OBJECTS)))

out/libreactor.a.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CXX   $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/libreactor.a: $(libreactor_OBJECTS)
	$Qmkdir -p $(@D)
	$Qrm -f $@
	$(if $Q,@echo "  AR    $@")
	$Q$(AR) cru $@ $^
