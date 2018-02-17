CPP = g++
CPPFLAGS = -std=c++14 -O3
EXENAME = spfind
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)


all: $(EXENAME)

$(EXENAME): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $^

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@
	$(CPP) -MM $(CPPFLAGS) $< -o $*.d

clean::
	rm -f *.o *.d *.a $(EXENAME)

