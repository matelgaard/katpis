@echo off

rmdir build /s /q
mkdir build
pushd build
cl /Zi /Fe:main /nologo ../%1
popd
