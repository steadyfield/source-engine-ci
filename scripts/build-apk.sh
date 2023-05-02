cd modlauncher-waf
git clone https://gitlab.com/LostGamer/android-sdk
export ANDROID_SDK_HOME=$PWD/android-sdk
export MODNAME=missinginfo
export MODNAMESTRING=MissingInformation
git pull
./mod.sh
./waf configure -T release &&
./waf build
