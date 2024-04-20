# Julia Rendering


## Installs
```
sudo dnf install cmake
sudo dnf install SFML-devel
yum groupinstall "X Software Development"
```


## Make and run
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/FractalProject
```