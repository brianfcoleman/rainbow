# OS
sys := $(shell uname -s)

ifeq ($(sys), MINGW32_NT-6.1)
  os = WINNT
  compiler = msvc
  cxx = cl
	exe = exe
else
  $(error Sorry, your os is unknown/unsupported: $(sys))
endif

# Arch
machine := $(shell uname -m)

ifeq ($(machine), i386)
  arch = x86
else
ifeq ($(machine), i586)
  arch = x86
else
ifeq ($(machine), i686)
  arch = x86
else
ifeq ($(machine), x86_64)
  arch = x86
else
  $(error: Sorry, your architecture is unknown/unsupported: $(machine))
endif
endif
endif
endif

# Target and objects
target = VideoCaptureTest
exe_target = $(target:=.$(exe))
cpp_objects = $(cpp_sources:.cpp=.o)

# source and path configurations
cpp_sources = VideoCaptureTest.cpp
cpp_sources += $(wildcard ../*.cpp)

sdkdir ?= ${MOZSDKDIR}
ifeq ($(os), WINNT)
	xpidl = $(sdkdir)/sdk/bin/xpidl
endif

# FIXME: we don't actually require this for e.g. clean
ifeq ($(sdkdir),)
  $(warning No 'sdkdir' variable given)
  $(warning It should point to the location of the Gecko SDK)
  $(warning For example: "make sdkdir=/foo/bar/baz")
  $(warning Or set the MOZSDKDIR environment variable to point to it)
  $(error )
endif

cpp_objects = $(cpp_sources:.cpp=.o)
exe_target = $(target:=.$(exe))

headers = -I. -I$(sdkdir)/include \
    -I$(sdkdir)/include/system_wrappers \
    -I$(sdkdir)/include/xpcom \
    -I$(sdkdir)/include/string \
    -I$(sdkdir)/include/content \
    -I$(sdkdir)/include/dom \
    -I$(sdkdir)/include/nspr \
    -I$(sdkdir)/sdk/include
headers += -I..

# libraries
libdirs := $(sdkdir)/lib $(sdkdir)/bin
libs := xpcomglue_s xpcom nspr4 mozalloc plds4 plc4

# compiler and Linker Flags
ifeq ($(os), WINNT)
  libdirs := $(patsubst %,-LIBPATH:%,$(libdirs))
  libs := $(patsubst %,$(sdkdir)/lib/%.lib,$(libs))
	libdir_winnt_x86_msvc := ../../lib/WINNT_x86-msvc9
  headers += -I../../lib/WINNT_x86-msvc/include

  ifeq ($(debug),1)
    cppflags += -MTd -DDEBUG
    ldops = msvcrtd.lib \
        /NODEFAULTLIB:libcmt /NODEFAULTLIB:msvcrt /NODEFAULTLIB:libcmtd
  else
    cppflags += -MT
    ldops = msvcrt.lib /NODEFAULTLIB:libcmt
  endif

  cppflags += -c -nologo -O1 -GR- -TP -Zc:wchar_t- -W3 -Gy $(headers) \
    -DTRIMMED -D_CRT_SECURE_NO_DEPRECATE=1 \
    -D_CRT_NONSTDC_NO_DEPRECATE=1 -DWINVER=0x500 -D_WIN32_WINNT=0x500 \
    -D_WIN32_IE=0x0500 -DX_DISPLAY_MISSING=1 -DMOZILLA_VERSION=\"1.9pre\" \
    -DMOZILLA_VERSION_U=1.9pre -DHAVE_SNPRINTF=1 -D_WINDOWS=1 -D_WIN32=1 \
    -DWIN32=1 -DXP_WIN=1 -DXP_WIN32=1 -DHW_THREADS=1 -DSTDC_HEADERS=1 \
    -DWIN32_LEAN_AND_MEAN=1 -DNO_X11=1 -DHAVE_MMINTRIN_H=1 \
    -DHAVE_OLEACC_IDL=1 -DHAVE_ATLBASE_H=1 -DHAVE_WPCAPI_H=1 -D_X86_=1 \
    -DD_INO=d_ino -DMOZ_NO_MOZALLOC=1 -DVIDEO_CAPTURE_DEBUG
  ldflags += -NOLOGO -NXCOMPAT -SAFESEH \
    $(libdirs) $(libs) \
    strmiids.lib advapi32.lib $(ldops)
rcflags := -r $(headers)
endif

######################################################################

.PHONY: all build clean

all: build

build: $(exe_target)

clean:
	rm -f $(exe_target) $(cpp_objects)

ifeq ($(os), WINNT)
  $(cpp_objects): $(cpp_sources)
	$(cxx) -Fo$@ -Fd$(@:.o=.pdb) $(cppflags) $(@:.o=.cpp)

  $(exe_target): $(cpp_objects)
	link -OUT:$@ -PDB:$(@:.exe=.pdb) $(cpp_objects) $(ldflags)
	chmod +x $@
endif

