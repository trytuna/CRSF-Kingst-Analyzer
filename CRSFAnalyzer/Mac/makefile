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

$(TARGET) : $(HFILE) $(SRC)
	$(CC) -target x86_64-apple-darwin $(CXXFLAGS) $(FPIC) $(SRC) $(INC)
	$(CC) -target x86_64-apple-darwin $(SHARE) $(TARGET) $(OBJ) $(LINK)

clean:
	rm -f *.o *.dylib

install:
	cp $(TARGET) /Applications/KingstVIS.app/Contents/Resources/Analyzer/
