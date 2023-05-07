cd modlauncher-waf
git clone https://gitlab.com/LostGamer/android-sdk
export ANDROID_SDK_HOME=$PWD/android-sdk
export MODNAME=smod13
export MODNAMESTRING=SMOD13
git pull
sudo apt install -y imagemagick
./mod.sh
wget https://raw.githubusercontent.com/steadyfield/srceng-android/android-fixes/res/drawable-xxxhdpi/ic_launcher.png
mv ic_launcher.png android/
./android/scripts/conv.sh android/ic_launcher.png
cp -r res android/
./waf configure -T release &&
./waf build