export ICON=gunmod-192.png
export PACKAGE=gunmod
export APP_NAME="GUNMOD: 0.5 (DF)"
cd srceng-mod-launcher
git clone https://gitlab.com/LostGamer/android-sdk
export ANDROID_SDK_HOME=$PWD/android-sdk
git pull
chmod +x android/script.sh
./android/scripts/script.sh
chmod +x waf
./waf configure -T release &&
./waf build
