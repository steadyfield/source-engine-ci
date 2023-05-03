cd modlauncher-waf
git clone https://gitlab.com/LostGamer/android-sdk
export ANDROID_SDK_HOME=$PWD/android-sdk
export MODNAME=missinginfo
export MODNAMESTRING=MissingInformation
git pull
sudo apt install -y imagemagick
./mod.sh
wget https://raw.githubusercontent.com/ItzVladik/extras/main/mi_logo.png
mv mi_logo.png android/
./android/scripts/conv.sh android/mi_logo.png
mv res android/
./waf configure -T release &&
./waf build
