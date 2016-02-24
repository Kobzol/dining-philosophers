ifndef $(VER)
  VER=32
  ifeq ($(shell uname -m),x86_64)
    VER=64
  endif
endif

TARGETS=holic_cl filozof_cl ctespis_cl

all: precomp $(TARGETS)

-include Makefile2
precomp: $(PRECOMP)

holic_cl: holic_cl.cpp msg_io.cpp protocol.o$(VER)
	g++ $^ -o $@

filozof_cl: filozof_cl.cpp msg_io.cpp protocol.o$(VER)
	g++ $^ -o $@

ctespis_cl: ctespis_cl.cpp msg_io.cpp protocol.o$(VER)
	g++ $^ -o $@

clean: $(CLEAN)
	rm -f $(TARGETS)
