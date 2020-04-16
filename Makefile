decode : decode.o
				g++ -o out/decode out/decode.o
decode.o :
				g++ -c src/decode.cpp -o out/decode.o  -std=c++2a
clean :
				rm -Rf out/