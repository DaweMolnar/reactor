# libreactor.mk

libreactor_SOURCES := \
	src/Host.cc \
	src/Ip.cc \
	src/Service.cc \
	src/Socket.cc \
	src/Timer.cc \
	src/Timers.cc \
	src/PollDemuxer.cc \
	src/Dispatcher.cc \
	src/Backlog.cc \
	src/Reactor.cc \
	src/Thread.cc \
	src/ThreadPool.cc \
	src/ThreadMutex.cc \
	src/ThreadCondition.cc \
	src/pthread/PthreadImpl.cc \
	src/pthread/PthreadMutexImpl.cc \
	src/pthread/PthreadConditionImpl.cc \
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
