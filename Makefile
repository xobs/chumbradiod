# chumbhttpd wrapper makefile
#

EXTRA_LIBS=$(IMPORT_BASE)/libs/$(TARGET)/lib $(PREFIX)/lib
EXTRA_INCLUDES=$(IMPORT_BASE)/libs/all/all/include $(PREFIX)/include
EXTRA_LIBS_PATH:=$(shell echo "$(EXTRA_LIBS)" | sed 's/[ ]\+/:/g')
# This is a bit cleaner than sticking it in front of the configure exec but doesn't work
#export CFLAGS:=$(CFLAGS) -Wl,-rpath-link -Wl,$(EXTRA_LIBS_PATH)
#export LDFLAGS=-rpath-link $(EXTRA_LIBS_PATH) $(addprefix -L,$(EXTRA_LIBS))
#export CPPFLAGS=$(addprefix -I,$(EXTRA_INCLUDES))

# Note that we need tu do this to get the linker to resolve so path refs, but need to
# wrap the linker commands which the front end doesn't recognize in -Wl,
RP_PRE=-Wl,--rpath-link -Wl,

# Check for location of doxygen, if exists
DOXYGEN:=$(shell which doxygen)

all: build

build: config.$(TARGET)
	$(MAKE) -C src

config.$(TARGET): Makefile
ifeq ($(PREFIX),)
	@echo "PREFIX not specified"
	@exit 1
else
	@echo "----------[ Configuring ]----------"
	@echo "Removing config status for other targets..."
	-rm config*
	-$(MAKE) -C src clean
	cd src ; \
		CPPFLAGS="$(addprefix -I,$(EXTRA_INCLUDES))" \
		CFLAGS="$(addprefix $(RP_PRE),$(EXTRA_LIBS))" \
		CXXFLAGS="$(addprefix $(RP_PRE),$(EXTRA_LIBS))" \
		LDFLAGS="$(addprefix -L,$(EXTRA_LIBS))" \
		 ./configure --prefix=$(PREFIX) --host=$(TARGET)
	touch $@
	@echo "====[ Configuration completed ]===="
endif

config: config.$(TARGET)

clean:
	-rm -rf config*
	-$(MAKE) -C src clean

install: build doxygen
	@echo "Installing to $(PREFIX)"
	# hgroover doesn't use arm-linux-strip
	$(MAKE) -C src install
	install -v -p -D chumbradiod.sh $(PREFIX)/etc/init.d/chumbradiod.sh
	install -v -p -D chumbradioplay.sh $(PREFIX)/etc/init.d/chumbradioplay.sh

doxygen:
ifneq ($(DOXYGEN),)
	@echo "Building doxygen"
	mkdir -p doc/doxygen/crad_interface
	doxygen doc/doxygen.conf
	mkdir -p $(PREFIX)/doc/doxygen/crad_interface/html/
	cp doc/doxygen/crad_interface/html/* $(PREFIX)/doc/doxygen/crad_interface/html/
endif

# config.$(TARGET) should NOT be phony
.PHONY: all clean install build doxygen config

