CPP=g++-8
CC =gcc-8
INSTALL_DIR = bin/
INCLUDE_DIR = -I. 

RM=/bin/rm

LIB      = libdaemon.so
LIBA     = libdaemon.a
SOURCE   = daemon.cpp
LINKLIBS = -lpthread -lstdc++fs

OBJS     = $(SOURCE:.cpp=.o) 
DEPS     = $(SOURCE:.cpp=.d) 

-include $(DEPS)

CFLAGSALL     = -std=c++17 -fPIC -Wall -Wextra -Werror -Wshadow -march=native -fno-default-inline -fno-stack-protector -pthread -Wall -Werror -pedantic -Wextra -Weffc++ -Waddress -Warray-bounds -Wno-builtin-macro-redefined -Wundef
CFLAGSRELEASE = -O2 -DNDEBUG $(CFLAGSALL)
CFLAGSDEBUG   = -ggdb3 -DDEBUG $(CFLAGSALL)

.PHONY: release
release: CFLAGS = $(CFLAGSRELEASE)
release: all

.PHONY: debug
debug: CFLAGS = $(CFLAGSDEBUG)
debug: all

.PHONY:clean
.PHONY:install

all:$(OBJS)
	$(CPP) -shared -o $(LIB) $(OBJS) $(LINKLIBS)
	ar rcs $(LIBA) $(OBJS)

%.o: %.cpp
	$(CPP) $(CFLAGS) $(INCLUDE_DIR) -MMD -MP -c $< -o $@

install: all
	install -d $(INSTALL_DIR)
	install -m 750 $(EXE) $(INSTALL_DIR)

uninstall:
	$(RM) -rf $(INSTALL_DIR)

clean:
	$(RM) -f *.o *.d $(LIB) $(LIBA) 
