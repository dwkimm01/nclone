CC=clang++
CFLAGS=-c -Wall
LDFLAGS=-stdlib=libc++
SOURCES=../src/NCApp.cpp \
../src/NCClientIf.cpp \
../src/NCClientPurple.cpp \
../src/NCCmdHistory.cpp \
../src/NCEnvironmentVariables.cpp \
../src/NCException.cpp \
../src/NCExceptionCurses.cpp \
../src/NCExceptionOutOfRange.cpp \
../src/NCMain.cpp \
../src/NCObject.cpp \
../src/NCString.cpp \
../src/NCTimeUtils.cpp \
../src/NCWin.cpp \
../src/NCWinBuffer.cpp \
../src/NCWinScrollback.cpp \
../src/NCWinTime.cpp

INCLUDES= -I../src -I/usr/local/include/libpurple -I/usr/local/Cellar/Boost/include -I/usr/local/lib/glib-2.0/include -I/usr/local/include/glib-2.0 -I/usr/local/include/glib-2.0/glib -g3 -fmessage-length=0 -std=c++11 -stdlib=libc++

LIBS= -lpurple \
-lglib-2.0 \
-lncurses \
-lboost_program_options \
-lboost_date_time \
-lboost_regex \
-lboost_system \
-lpthread \
-lboost_signals

LIBPATHS= -L/usr/local/Cellar/Boost/lib

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=NClone

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS) $(LIBPATHS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@


