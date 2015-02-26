NAME = ebctrans
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
LIBS = -lebc2json -latd -lshlwapi -lregex -lpsapi -lgdi32

CPPROOT = ..
ATDD = $(CPPROOT)/atd.d
AUID = $(CPPROOT)/aui.d
EBCD = $(CPPROOT)/ebc2json
REGEXD = $(CPPROOT)/regex-2.7-bin
INCDIRS = \
	-I $(ATDD) \
	-I $(AUID) \
	-I $(EBCD) \
	-I $(REGEXD)/include
LIBDIRS = \
	-L $(ATDD)/bin \
	-L $(AUID)/bin \
	-L $(EBCD)/bin \
	-L $(REGEXD)/lib
IMPLIBS = \
	$(ATDD)/bin/libatd.a \
	$(AUID)/bin/libaui.a \
	$(EBCD)/bin/libebc2json.a 

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