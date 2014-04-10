all : release

debug :
	mkdir -p bin-dbg
	cd bin-dbg && cmake -DCMAKE_BUILD_TYPE=Debug ../source && make

release :
	mkdir -p bin-opt
	cd bin-opt && cmake -DCMAKE_BUILD_TYPE=Release ../source && make

clean :
	rm -rf bin-opt
	rm -rf bin-dbg

install : debug
	cd bin-dbg && make install

install-release : release
	cd bin-opt && make install

uninstall : debug
	cd bin-dbg && make uninstall

uninstall-release : release
	cd bin-opt && make uninstall
