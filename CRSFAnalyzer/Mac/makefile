TARGET  := libCRSF.dylib

LINK := -L "../../lib/Mac" -lAnalyzer

CC       := clang++
HFILE    := ../../inc/*.h
SRC      := ../src/*.cpp
INC      := -I ../../inc/
CXXFLAGS := -Wall -O2 -c -Wno-unknown-pragmas
FPIC     := -fPIC
SHARE    := -dynamiclib -o
OBJ      := *.o

APPLICATION_PATH	:= /Applications/KingstVIS.app

all: libCRSF.dylib install

$(TARGET) : $(HFILE) $(SRC)
	$(CC) -target x86_64-apple-darwin $(CXXFLAGS) $(FPIC) $(SRC) $(INC)
	$(CC) -target x86_64-apple-darwin $(SHARE) $(TARGET) $(OBJ) $(LINK)

clean:
	rm -f *.o *.dylib

install:
	cp $(TARGET) $(APPLICATION_PATH)/Contents/Resources/Analyzer/

# Running the application from the command line could be handy to see logs from the analyzer code.
# There are no other debug methods than logging to stdout that I know of.
# 
# Run this command to speed up your development workflow: make install run
run:
	$(APPLICATION_PATH)/Contents/MacOS/KingstVIS
