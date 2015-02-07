NAME = ebc2json
BINDIR = ./bin
OBJDIR = ./obj
TARGET = $(BINDIR)/$(NAME).exe
SOURCES = $(wildcard *.cpp)
OBJECTS = $(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
DEPENDS = $(OBJECTS:.o=.d)
CXXFLAGS = -Wall -O3 -MMD -MP
LDFLAGS = -static
LIBS = -latd -lshlwapi -lregex

CPPROOT = ..
INCDIRS = \
	-I $(CPPROOT)/atd.d 
LIBDIRS = \
	-L $(CPPROOT)/atd.d/bin \
	-L $(CPPROOT)/regex-2.7-bin/lib

$(TARGET): $(OBJECTS) $(CPPROOT)/atd.d/bin/libatd.a
	@ [ -d $(BINDIR) ] | mkdir -p $(BINDIR)
	$(CXX) $(LDFLAGS) $(LIBDIRS) -o $@ $(OBJECTS) $(LIBS)
	strip $@ --strip-all

$(OBJDIR)/%.o: %.cpp
	@ [ -d $(OBJDIR) ] | mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o $@ -c $<

clean:
	$(RM) $(TARGET) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)