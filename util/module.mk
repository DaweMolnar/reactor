# util/module.mk

all: out/libutil.a

libutil_SOURCE_NAMES := \
	AutoFd.cc \
	DiffTime.cc \
	ErrnoException.cc \
	Fd.cc \
	Pipe.cc \
	Time.cc

libutil_SOURCES := $(addprefix util/,$(libutil_SOURCE_NAMES))
libutil_OBJECTS := $(sort $(addprefix out/libutil.a.d/,$(addsuffix .o,$(basename $(libutil_SOURCES)))))
-include $(addsuffix .d,$(basename $(libutil_OBJECTS)))

out/libutil.a.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/libutil.a: $(libutil_OBJECTS)
	$Qmkdir -p $(@D)
	$Qrm -f $@
	$(if $Q,@echo "  AR    $@")
	$Q$(AR) cru $@ $^
