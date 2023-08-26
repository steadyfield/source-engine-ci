git submodule init && git submodule update

echo "Setup dependencies"
sudo apt update -y
sudo apt install -y automake autoconf libtool build-essential gcc-multilib g++-multilib pkg-config ccache libsdl2-dev libfreetype6-dev libfontconfig1-dev libopenal-dev libjpeg-dev libpng-dev libcurl4-gnutls-dev libbz2-dev libedit-dev git

echo "Setup OPUS"
git clone --recursive --depth 1 https://github.com/xiph/opus
cd opus
./autogen.sh && ./configure --enable-custom-modes && make -j$(nproc) && sudo make install
cd ../

echo "Build"
mkdir -p built
python3 ./waf configure -T release --build-games sourcebox --64bits --prefix ./built --enable-opus
python3 ./waf install

echo "Assets"
git clone https://github.com/sourceboxgame/sourcebox-assets ./built/sourcebox
mv ./built/sourcebox/gameinfo.txt.template ./built/sourcebox_linux/gameinfo.txt
sed -i "s/PLATFORM/linux/g" ./built/sourcebox_linux/gameinfo.txt
