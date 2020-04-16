decode : decode.o ZeroCrossingDetector.o
				g++ -o out/decode out/decode.o out/ZeroCrossingDetector.o
decode.o :
				g++ -c src/decode.cpp -o out/decode.o  -std=c++2a
ZeroCrossingDetector.o :
				g++ -c src/ZeroCrossingDetector.cpp -o out/ZeroCrossingDetector.o  -std=c++2a
clean :
				rm -Rf out/