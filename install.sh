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
cp ./calcVF/bin/glew32.dll ./
mkdir -p ./shaders
cp ./calcVF/shaders/* ./shaders/
cp ./calcVF/README.md ./
rm -rf ./calcVF
