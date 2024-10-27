#!/bin/sh

git submodule init && git submodule update

brew install sdl2

./waf configure -T release --build-game=mapbase_episodic --prefix=bin --disable-warns $* &&
./waf install --target=client,server,game_shader_dx9 --strip
