# net/module.mk

all: out/libnet.a

libnet_SOURCE_NAMES := \
	Host.cc \
	Ip.cc \
	Service.cc

libnet_SOURCES := $(addprefix net/,$(libnet_SOURCE_NAMES))
libnet_OBJECTS := $(sort $(addprefix out/libnet.a.d/,$(addsuffix .o,$(basename $(libnet_SOURCES)))))
-include $(addsuffix .d,$(basename $(libnet_OBJECTS)))

out/libnet.a.d/%.o: %.cc
	$Qmkdir -p $(@D)
	$(if $Q,@echo "  CC    $@")
	$Q$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTPUT_OPTION)

out/libnet.a: $(libnet_OBJECTS)
	$Qmkdir -p $(@D)
	$Qrm -f $@
	$(if $Q,@echo "  AR    $@")
	$Q$(AR) cru $@ $^
