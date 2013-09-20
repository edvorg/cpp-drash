CPP_FLAGS=-std=c++11
INCLUDES=-I3rd/Box2D/include

drash-sdl-build/drash_sdl: drash.qtproject/drash_sdl.pro
	mkdir -p drash-sdl-build
	cd drash-sdl-build ; qmake ../drash.qtproject/drash_sdl.pro
	cd drash-sdl-build ; make

drash-build/drash: drash.qtproject/drash.pro
	mkdir -p drash-build
	cd drash-build ; qmake ../drash.qtproject/drash.pro
	cd drash-build ; make

drash-sdl: drash-sdl-build/drash_sdl
drash: drash-build/drash

drash-sdl-clean:
	rm -rf drash-sdl-build/*

drash-clean:
	rm -rf drash-build/*

check-syntax:
	clang++ ${CPP_FLAGS} ${INCLUDES} -fsyntax-only -fno-color-diagnostics ${CHK_SOURCES}

print-cflags:
	echo ${CPP_FLAGS} ${INCLUDES}
