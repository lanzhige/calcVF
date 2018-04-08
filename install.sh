git clone https://github.com/lanzhige/calcVF.git
cd ./calcVF
mkdir -p ./build
cd ./build
cmake ..
cmake --build . --config Release
cd ..
cd ..
cp ./calcVF/build/src/Release/calcVF.exe ./
cp ./calcVF/bin/zlib.dll ./
cp ./calcVF/README.md ./
rm -rf ./calcVF
