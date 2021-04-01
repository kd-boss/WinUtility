.PHONY: all

CFLAGS = -mwindows -municode -s -O3
LIBS = -lcrypt32 -lbcrypt

crypttest.exe:
	g++ cryptoexample.cpp $(CFLAGS) $(LIBS) -o crypttest.exe

.PHONY: clean
clean:

	-rm -f *.exe
