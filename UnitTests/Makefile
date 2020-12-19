SRCS=$(filter-out , $(wildcard *.cpp))
BIN=unittests
LIBS=
TEST_DEPS=gmock_main gtest gmock
TESTLIBS=$(shell pkg-config --libs --static $(TEST_DEPS))
CFLAGS+=$(shell pkg-config --cflags $(TEST_DEPS))
LDFLAGS=-L /usr/lib/avr/ -Wl,--gc-sections -lm -L $(INSTALL_DIR)/lib $(LIBS:%=-l%) $(TESTLIBS)

.PHONY : all clean 

all : $(BIN:%=$(INSTALL_DIR)/bin/%)
	@echo "compiling $<";

clean : 
	@echo "clean all-tst";
	rm -rf $(SRCS:.cpp=.o) $(BIN)

$(INSTALL_DIR)/bin/unittests: unittests
	-cp -f $^ $@;

unittests : $(SRCS:%.cpp=%.o)
	$(CXX) $(CFLAGS) $(SUBDIRS:%=-I ../%) -o $@ $^ $(LDFLAGS);

%.o : %.cpp
	$(CXX) $(CFLAGS) $(SUBDIRS:%= -I ../%) -o $@ -c $^;
