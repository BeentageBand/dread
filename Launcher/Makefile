SRCS=$(filter-out , $(wildcard *.cpp))
BIN=std-dread-launcher slv-dread-launcher
LIBS=\
		 app-card-reader app-connection-handler app-dread-client app-net-gateway \
		 support-fsm support-scheduler support-persistence support-router support-scheduler support-locale support-modem \
		 modules-sim900 modules-sd modules-sd-utility modules-rs485 modules-mifare modules-hid \
		 drivers-gpio drivers-hardware-serial drivers-software-serial drivers-timers drivers-spi \
		 dread-utilities

LDFLAGS=-L /usr/lib/avr/ -Wl,--gc-sections -lm -L $(INSTALL_DIR)/lib $(LIBS:%=-l%)

.PHONY : all clean 

all : $(BIN:%=$(INSTALL_DIR)/bin/%)
	@echo "compiling $<";

clean : 
	@echo "clean all-tst";
	rm -rf $(SRCS:.cpp=.o) $(BIN)

$(INSTALL_DIR)/bin/%-launcher : %-launcher
	-cp -f $^ $@;

%-launcher : %-launcher.o
	$(CXX) $(CFLAGS) $(SUBDIRS:%=-I ../%) -o $@ $^ $(LDFLAGS);

%.o : %.cpp
	$(CXX) $(CFLAGS) $(SUBDIRS:%= -I ../%) -o $@ -c $^;
