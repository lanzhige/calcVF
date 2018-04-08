git clone https://github.com/lanzhige/calcVF.git
cd ./calcSVF
mkdir -p ./build
cd ./build
cmake ..
cmake --build . --config Release
cd ..
cd ..
cp ./calcSVF/build/src/Release/calcSVF.exe ./
cp ./calcSVF/bin/zlib.dll ./
cp ./calcSVF/README.md ./
rm -rf ./calcSVF
