CXX		= g++-4.8
CXXFLAGS	= -std=c++11 -ggdb
EXECUTABLE  = scheme_interpreter
EXTENSION   = cpp
SOURCES	 = $(basename $(shell find . -name '*.$(EXTENSION)'))

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(addsuffix .o,$(SOURCES))
	$(CXX) $(LDFLAGS) $^ -o $@

%.d: %.$(EXTENSION)
	$(CXX) $(CXXFLAGS) -MM $< -o $@

%.o: %.cpp
	$(CXX) -std=c++11 -c $< -o $@

-include $(addsuffix .d,$(SOURCES))

.PHONY: clean
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*~' -delete
	find . -name '*.gch' -delete
