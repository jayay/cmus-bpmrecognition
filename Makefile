REV	= HEAD

# version from an annotated tag
_ver0	= $(shell git describe $(REV) 2>/dev/null)
# version from a plain tag
_ver1	= $(shell git describe --tags $(REV) 2>/dev/null)
# SHA1
_ver2	= $(shell git rev-parse --verify --short $(REV) 2>/dev/null)
# hand-made
_ver3	= v1.0.0

# LDFLAGS+= ""

# Commands taken from cmus

VERSION	= $(or $(_ver0),$(_ver1),$(_ver2),$(_ver3))
OBJDIR	= build
VPATH	= src
LD	= $(CC)

$(OBJDIR)/%.o: %.S
	$(call cmd,as)

$(OBJDIR)/%.o: %.c
	$(call cmd,cc)

$(OBJDIR)/%.o: %.cpp
	$(call cmd,cxx)

CFLAGS += -Iinclude -Wall
CPPFLAGS += -I/usr/local/include/soundtouch -Iinclude -Wall
LDFLAGS +=-ldl -lpthread -lm -L/usr/lib64 -L/usr/local/lib/ -lSoundTouch -Wall

quiet_cmd_cc    = CC     $@
      cmd_cc    = $(CC) -c $(CFLAGS) -o $@ $<

quiet_cmd_cxx    = CXX     $@
      cmd_cxx    = $(CXX) -c $(CFLAGS) -o $@ $^ $(CPPFLAGS)

# HOSTCC for program object files (.o)
quiet_cmd_hostcc    = HOSTCC     $@
      cmd_hostcc    = $(HOSTCC) -c $(HOST_CFLAGS) -o $@ $<

# CC for shared library and dynamically loadable module objects (.lo)
quiet_cmd_cc_lo = CC     $@
      cmd_cc_lo = $(CC) -c $(CPPFLAGS) $(CFLAGS) $(SOFLAGS) -o $@ $<

# LD for programs, optional parameter: libraries
quiet_cmd_ld = LD     $@
      cmd_ld = $(LD) $(LDFLAGS) -o $@ $^ $(1) -lc -lstdc++

# HOSTLD for programs, optional parameter: libraries
quiet_cmd_hostld = HOSTLD     $@
      cmd_hostld = $(HOSTLD) $(HOST_LDFLAGS) -o $@ $^ $(1)

# LD for shared libraries, optional parameter: libraries
quiet_cmd_ld_so = LD     $@
      cmd_ld_so = $(LD) $(LDSOFLAGS) $(LDFLAGS) -o $@ $^ $(1)

# LD for dynamically loadable modules, optional parameter: libraries
quiet_cmd_ld_dl = LD     $@
      cmd_ld_dl = $(LD) $(LDDLFLAGS) $(LDFLAGS) -o $@ $^ $(1)

cmd = @$(if $($(quiet)cmd_$(1)),echo '   $(call $(quiet)cmd_$(1),$(2))' &&) $(call cmd_$(1),$(2))

all: CXXFLAGS += -O3
all: CFLAGS += -O3
all: $(OBJDIR)/cmus-bpmrecognition

debug: CXXFLAGS += -DDEBUG -g -O0
debug: CCFLAGS += -DDEBUG -g -O0
debug: LDFLAGS += -DDEBUG -g -O0
debug: $(OBJDIR)/cmus-bpmrecognition

$(OBJDIR)/cmus-bpmrecognition: $(addprefix $(OBJDIR)/, base64.o bpmread.o buffer.o cache.o channelmap.o comment.o convert.o debug.o file.o gbuf.o input.o keyval.o locking.o main.o mergesort.o misc.o output.o path.o pcm.o player.o prog.o rbtree.o soundtouch-wrapper.o track_info.o uchar.o u_collate.o xmalloc.o xstrjoin.o)
	$(call cmd,ld)
.PHONY: clean
clean:
	$(RM) build/*.o build/cmus-bpmrecognition
