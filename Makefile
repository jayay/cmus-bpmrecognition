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



# Build verbosity:
#   make V=0    silent
#   make V=1    clean (default)
#   make V=2    verbose

# build verbosity (0-2), default is 1

ifneq ($(origin V),command line)
  V := 1
endif
ifneq ($(findstring s,$(MAKEFLAGS)),)
  V := 0
endif

ifeq ($(V),2)
  quiet =
  Q =
else
  ifeq ($(V),1)
    quiet = quiet_
    Q = @
  else
    quiet = silent_
    Q = @
  endif
endif

VERSION	= $(or $(_ver0),$(_ver1),$(_ver2),$(_ver3))
OBJDIR	= build
VPATH	= src
LD	= $(CC)

$(OBJDIR)/%.o: %.c
	$(call cmd,cc)

$(OBJDIR)/%.o: %.cpp
	$(call cmd,cxx)

CFLAGS += -Iinclude $(shell pkg-config aubio --cflags) -Wall
LDFLAGS += -ldl -lpthread -lm -L/usr/lib64 -L/usr/local/lib/ $(shell pkg-config aubio --libs-only-l) -Wall -rdynamic

quiet_cmd_cc	= CC	$@
      cmd_cc	= $(CC) -c $(CFLAGS) -o $@ $<

# LD for programs, optional parameter: libraries
quiet_cmd_ld	= LD	$@
      cmd_ld	= $(LD) $(LDFLAGS) -o $@ $^ $(1) -lc

cmd = @$(if $($(quiet)cmd_$(1)),echo '   $(call $(quiet)cmd_$(1),$(2))' &&) $(call cmd_$(1),$(2))

all: CFLAGS += -O3
all: $(OBJDIR)/cmus-bpmrecognition

debug: CFLAGS += -g -O0 -DDEBUG
debug: LDFLAGS += -g -O0 -DDEBUG
debug: quiet =
debug: $(OBJDIR)/cmus-bpmrecognition

bintostdout: CFLAGS += -DBINTOSTDOUT
bintostdout: debug

$(OBJDIR)/cmus-bpmrecognition: $(addprefix $(OBJDIR)/, ape.o base64.o bpmread.o buffer.o cache.o channelmap.o comment.o convert.o debug.o file.o gbuf.o id3.o input.o keyval.o locking.o main.o mergesort.o misc.o output.o path.o pcm.o player.o prog.o rbtree.o read_wrapper.o sampleconvert.o track_info.o uchar.o u_collate.o xmalloc.o xstrjoin.o)
	$(call cmd,ld)
.PHONY: clean debug
clean:
	$(RM) build/*.o build/cmus-bpmrecognition
