# libreactor.mk

libreactor_SOURCES := \
	src/Socket.cc \
	src/Timer.cc \
	src/Timers.cc \
	src/PollDemuxer.cc \
	src/Dispatcher.cc \
	src/Backlog.cc \
	src/Reactor.cc \
	src/MultiReactor.cc \
	src/Client.cc

libreactor_OBJECTS := $(sort $(addprefix out/libreactor.a.d/,$(addsuffix .o,$(basename $(libreactor_SOURCES)))))
-include $(addsuffix .d,$(basename $(libreactor_OBJECTS)))

out/libreactor.a.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/libreactor.a: $(libreactor_OBJECTS)
	$Qmkdir -p $(@D)
	$Qrm -f $@
	$(if $Q,@echo "  AR    $@")
	$Q$(AR) cru $@ $^
