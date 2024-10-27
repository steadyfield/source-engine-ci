#!/bin/sh

git submodule init && git submodule update

brew install sdl2

./waf configure -T release --build-game=tf_port --prefix=bin --disable-warns $* &&
./waf install --strip
