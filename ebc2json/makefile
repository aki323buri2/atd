NAME = ebc2json
BINDIR = ./bin
OBJDIR = ./obj
TARGET = $(BINDIR)/$(NAME).dll
EXEOUT = $(BINDIR)/$(NAME).exe

SOURCES = $(wildcard *.cpp)
OBJECTS = $(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
DEPENDS = $(OBJECTS:.o=.d)
CXXFLAGS = -Wall -O3 -MMD -MP
DLLLDFLAGS = -static -shared
EXELDFLAGS = -static
LIBS = -latd -lshlwapi -lregex

CPPROOT = ..
INCDIRS = \
	-I $(CPPROOT)/atd.d 
LIBDIRS = \
	-L $(CPPROOT)/atd.d/bin \
	-L $(CPPROOT)/regex-2.7-bin/lib

IMPLIBS = \
	$(CPPROOT)/atd.d/bin/libatd.a \

$(TARGET): $(OBJECTS) $(IMPLIBS) $(EXEOUT)
	@ [ -d $(BINDIR) ] | mkdir -p $(BINDIR)
	$(CXX) $(DLLLDFLAGS) $(LIBDIRS) -o $@ $(OBJECTS) $(LIBS)
	strip $@ --strip-all
	#静的ライブラリ
	ar rsv $(BINDIR)/lib$(NAME).a $^

$(EXEOUT): $(OBJECTS) $(IMPLIBS)
	@ [ -d $(BINDIR) ] | mkdir -p $(BINDIR)
	$(CXX) $(EXELDFLAGS) $(LIBDIRS) -o $@ $(OBJECTS) $(LIBS)
	strip $@ --strip-all

$(OBJDIR)/%.o: %.cpp
	@ [ -d $(OBJDIR) ] | mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o $@ -c $<

clean:
	$(RM) $(TARGET) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)