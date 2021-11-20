# The following two path works only if you have run
# inside the ardour6 git clone folder :
#
#		sudo ./waf install
#		sudo mkdir /usr/local/include/ardour6
#		sudo cp -r ./libs/* /usr/local/include/ardour6/
#		sudo cp ./build/libs/pbd/pbd/signals_generated.h /usr/local/include/ardour6/
#
# Or simply copy ardour_export.cc and ardour_aafimport.cc
# to session_utils/ and re-run ./waf
ARDOUR_INCLUDE_PATH = /usr/local/include/ardour6
ARDOUR_LIB_PATH = /usr/local/lib/ardour6
# ARDOUR_INCLUDE_PATH = /path/to/ardour6-git-clone/libs
# ARDOUR_LIB_PATH = /path/to/ardour6-git-clone/build/libs


CC = gcc
# CFLAGS = -W -Wall -g -O3 -lpthread -lwebsockets -lssl -lcrypto -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion -Wswitch-default -Wundef -Werror -Wno-unused  #-Wstrict-overflow=5
SRC = ./
BINDIR = ./bin

PP = g++
CPPFLAGS = -g -Wall -Wextra -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wno-unused
ARDOUR_LIBS = \
`pkg-config gtkmm-2.4 --cflags --libs` \
`pkg-config libxml-2.0 --cflags --libs` \
-I $(ARDOUR_INCLUDE_PATH)/ \
-I $(ARDOUR_INCLUDE_PATH)/ardour/ \
-I $(ARDOUR_INCLUDE_PATH)/pbd/ \
-I $(ARDOUR_INCLUDE_PATH)/pbd/pbd/ \
-I $(ARDOUR_INCLUDE_PATH)/temporal/ \
-I $(ARDOUR_INCLUDE_PATH)/evoral/ \
-I $(ARDOUR_INCLUDE_PATH)/midi++2/ \
-I $(ARDOUR_INCLUDE_PATH)/libltc/ltc/ \
-I $(ARDOUR_INCLUDE_PATH)/lua/ \
-I $(ARDOUR_INCLUDE_PATH)/audiographer/ \
-L $(ARDOUR_LIB_PATH)/ \
-L $(ARDOUR_LIB_PATH)/*.so* \
-L $(ARDOUR_LIB_PATH)/backends/lib*.so \
-L $(ARDOUR_LIB_PATH)/engines/libclearlooks.so \
-L $(ARDOUR_LIB_PATH)/vamp/libardourvampp*\



all: $(BINDIR)/_ardour_aafimport

$(BINDIR)/_ardour_aafimport: $(SRC)/ardour_aafimport.cc $(SRC)/common.cc $(SRC)/fileops.c
	$(PP) -o $@ $^ $(CPPFLAGS) $(ARDOUR_LIBS) -lAAF
	chmod +x $(BINDIR)/ardour_aafimport


clean:
	-rm -f $(BINDIR)/_ardour_aafimport
	@echo
