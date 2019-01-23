mkdir build
cd build
cmake ..
if [ "$PLATFORM" = "nix" ]; then
make -j
else
MSBuild.exe ALL_BUILD.vcxproj -m
fi
