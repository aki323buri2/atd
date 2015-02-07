NAME = atd
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
LIBS = -lshlwapi -lws2_32 -lwininet -lssl.dll -lcrypto.dll -lregex

CPPROOT = ..
INCDIRS = \
	-I $(CPPROOT)/openssl-install/include \
	-I $(CPPROOT)/regex-2.7-bin/include
LIBDIRS = \
	-L $(CPPROOT)/openssl-install/lib \
	-L $(CPPROOT)/regex-2.7-bin/lib


$(TARGET): $(OBJECTS) $(EXEOUT)
	@ [ -d $(BINDIR) ] | mkdir -p $(BINDIR)
	$(CXX) $(DLLLDFLAGS) $(LIBDIRS) -o $@ $^ $(LIBS) 
	strip $@ --strip-all
	#静的ライブラリ
	ar rsv $(BINDIR)/lib$(NAME).a $^

$(EXEOUT): $(OBJECTS)
	@ [ -d $(BINDIR) ] | mkdir -p $(BINDIR)
	$(CXX) $(EXELDFLAGS) $(LIBDIRS) -o $@ $^ $(LIBS) 
	strip $@ --strip-all

$(OBJDIR)/%.o: %.cpp
	@ [ -d $(OBJDIR) ] | mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o $@ -c $< 

clean: 
	$(RM) $(TARGET) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)
