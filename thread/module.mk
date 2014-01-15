# thread/module.mk

all: out/libthread.a

libthread_SOURCE_NAMES := \
	pthread/PthreadConditionImpl.cc \
	pthread/PthreadImpl.cc \
	pthread/PthreadMutexImpl.cc \
	Thread.cc \
	ThreadCondition.cc \
	ThreadMutex.cc \
	ThreadPool.cc

libthread_SOURCES := $(addprefix thread/,$(libthread_SOURCE_NAMES))
libthread_OBJECTS := $(sort $(addprefix out/libthread.a.d/,$(addsuffix .o,$(basename $(libthread_SOURCES)))))
-include $(addsuffix .d,$(basename $(libthread_OBJECTS)))

out/libthread.a.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/libthread.a: $(libthread_OBJECTS)
	$Qmkdir -p $(@D)
	$Qrm -f $@
	$(if $Q,@echo "  AR    $@")
	$Q$(AR) cru $@ $^