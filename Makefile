CXX = g++
CXXFLAGS = -Wall -g
SRCS = src/unit_test.cc
OBJS = unit_test.o
TARGET = unit_test
RM = rm -f

.PHONY: all clean

$(TARGET): $(OBJS)
	$(CXX) -o $@ $?

$(OBJS): %.o : src/%.cc
	$(CXX) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS)

