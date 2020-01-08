CPP=g++-8

INSTALL_DIR = ../bin
INCLUDE_DIR = -I. -I../

RM=/bin/rm

EXE      = test-daemon
SOURCE   = sample-daemon.cpp
LINKLIBS = -lpthread -L../ -ldaemon

OBJS     = $(SOURCE:.cpp=.o) 
DEPS     = $(SOURCE:.cpp=.d) 

-include $(DEPS)

DIRS = ../

CFLAGSALL     = -std=c++17 -Wall -Wextra -Werror -Wshadow -march=native -fno-default-inline -fno-stack-protector -pthread -Wall -Werror -pedantic -Wextra -Weffc++ -Waddress -Warray-bounds -Wno-builtin-macro-redefined -Wundef
CFLAGSRELEASE = -O2 -DNDEBUG $(CFLAGSALL)
CFLAGSDEBUG   = -ggdb3 -DDEBUG $(CFLAGSALL)

.PHONY: release
release: CFLAGS = $(CFLAGSRELEASE)
release: TYP=release
release: all

.PHONY: debug
debug: CFLAGS = $(CFLAGSDEBUG)
debug: TYP=debug
debug: all

.PHONY:clean
.PHONY:install

all: $(DIRS)

all: $(OBJS)
	cd .. && $(MAKE) $(TYP)
	$(CPP) -o $(EXE) $(OBJS) $(LINKLIBS)

%.o: %.cpp
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -MMD -MP -c $< -o $@

install : all
	install -d $(INSTALL_DIR)
	install -m 750 $(EXE) $(INSTALL_DIR)

uninstall:
	$(RM) -rf $(INSTALL_DIR)

clean:
	$(RM) -f *.o *.d $(EXE) 
	cd .. && $(MAKE) clean
