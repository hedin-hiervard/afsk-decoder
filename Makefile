decode : decode.o
				cc -o out/decode out/decode.o
decode.o :
				cc -c src/decode.cpp -o out/decode.o  -std=c++2a
clean :
				rm -Rf out/