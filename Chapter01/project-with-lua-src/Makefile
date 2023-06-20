project-with-lua-src: main.cpp
	cd lua && make
	g++ -o project-with-lua-src main.cpp -Ilua -Llua -llua

clean:
	rm -f project-with-lua-src
	cd lua && make clean
