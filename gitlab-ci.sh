apt update
apt install sudo
cp scripts/Linux/* .
chmod u+x ./*.sh
./downloadPackages.sh 
git submodule update --init
./rebuildElpida.sh
ll apps/Qt/