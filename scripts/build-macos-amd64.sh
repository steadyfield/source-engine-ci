#!/bin/sh

git submodule init && git submodule update

brew install sdl2

./waf configure -T release --disable-warns  --build-games=gunmod --prefix=bin $* &&
./waf install --strip
