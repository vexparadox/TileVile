echo "Your password will be required to use this script, it installs to system directories."
echo "Made by William Meaton > http://willmeaton.com"
echo "----------------------------------------------"
sudo echo "Making temporary folder ./axinstall"
mkdir -p axinstall
cd ./axinstall
echo "Downloading Axilya..."
curl -Lks https://github.com/vexparadox/Axilya/archive/master.zip -o Axilya-master.zip
if [ "$(uname)" = "Darwin" ]; then
	echo "Detected OSX"
	unzip -qq Axilya-master.zip
	cd Axilya-master
	echo "Running dependency script..."
	sh ./OSXDependenciesInstall.sh
	echo "Installing Axilya..."
	sudo make install
	cd ../
	rm Axilya-master.zip
	sudo rm -rf Axilya-master
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
	echo "Detected Linux"
	echo "Checking for unzip..."
	sudo apt-get -qq -y install unzip;
	unzip -qq Axilya-master
	cd Axilya-master
	echo "Running dependency script..."
	sh ./LinuxDependenciesInstall.sh
	echo "Install Axilya..."
	sudo make install
	echo "Building Axilya Examples..."
	make examples
	cd ../
	rm Axilya-master.zip
	cp -r Axilya-master/Examples Examples
	sudo rm -rf Axilya-master
fi

echo "Axilya and dependencies installed!"
echo "See http://axilya.com for more information."