CPP=g++-8
CC =gcc-8
INSTALL_DIR     = /usr/lib/libdaemon/
INCLUDE_DIR     = -I. 
INCLUDE_INSTALL = /usr/include/libdaemon

RM=/bin/rm
RMDIR=/bin/rmdir

LIB      = libdaemon.so
LIBA     = libdaemon.a
SOURCE   = daemon.cpp
HEADER   = daemon.h
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
	install -d $(INCLUDE_INSTALL)
	install -m 644 $(LIB) $(INSTALL_DIR)
	install -m 644 $(LIBA) $(INSTALL_DIR)
	install -m 644 $(HEADER) $(INCLUDE_INSTALL)

uninstall:
	$(RM) -f $(INSTALL_DIR)/$(LIB)
	$(RM) -f $(INSTALL_DIR)/$(LIBA)
	$(RM) -f $(INCLUDE_INSTALL)/$(HEADER)
	$(RMDIR) -v $(INSTALL_DIR)
	$(RMDIR) -v $(INCLUDE_INSTALL)

clean:
	$(RM) -f *.o *.d $(LIB) $(LIBA) 
