TARGET ?= solarEdgeAntiApi

SRCS := $(shell find *.cpp)
OBJS := $(addsuffix .o, $(basename $(SRCS)))

LIBS += -lcurl -lpthread


$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) 

-include $(DEPS)
