export ICON=ez1.png
export PACKAGE=EntropyZero
export APP_NAME="Entropy Zero"
cd srceng-mod-launcher
git clone https://gitlab.com/LostGamer/android-sdk
export ANDROID_SDK_HOME=$PWD/android-sdk
git pull
chmod +x android/script.sh
./android/scripts/script.sh
chmod +x waf
./waf configure -T release --javac-source-target=8 &&
./waf build
