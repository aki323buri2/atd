NAME = aui
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
LIBS = -latd -lshlwapi -lregex -lpsapi -lgdi32

CPPROOT = ../
ATDD = $(CPPROOT)/atd.d
REGEXD = $(CPPROOT)/regex-2.7-bin
INCDIRS = \
	-I $(ATDD) \
	-I $(AUID) \
	-I $(REGEXD)/include
LIBDIRS = \
	-L $(ATDD)/bin \
	-L $(AUID)/bin \
	-L $(REGEXD)/lib

$(TARGET): $(OBJECTS) $(ATDD)/bin/libatd.a $(IMPLIB) $(EXEOUT)
	@ [ -d $(BINDIR) ] | mkdir -p $(BINDIR)
	$(CXX) $(DLLLDFLAGS) $(LIBDIRS) -o $@ $(OBJECTS) $(LIBS)
	strip $@ --strip-all 
	#静的ライブラリ
	ar rsv $(BINDIR)/lib$(NAME).a $^

$(EXEOUT): $(OBJECTS) $(ATDD)/bin/libatd.a $(IMPLIB)
	@ [ -d $(BINDIR) ] | mkdir -p $(BINDIR)
	$(CXX) $(EXELDFLAGS) $(LIBDIRS) -o $@ $(OBJECTS) $(LIBS)
	strip $@ --strip-all 


$(OBJDIR)/%.o: %.cpp
	@ [ -d $(OBJDIR) ] | mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o $@ -c $<

clean:
	$(RM) $(TARGET) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)