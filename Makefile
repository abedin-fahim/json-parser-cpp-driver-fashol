remove:
	rm -rf build && mkdir build && cd build && cmake .. && make && cd ..

remove-deb: 
	rm ./fashol-json-parser-1.0.0.deb

build-deb:
	dpkg-deb --build debian ./fashol-json-parser-1.0.0.deb

remove-tar: 
	rm ../fashol-json-parser_1.0.0.orig.tar.gz

build-tar:
	tar --exclude-vcs -czf ../fashol-json-parser_1.0.0.orig.tar.gz .

build-driver: 
	debuild -us -uc
