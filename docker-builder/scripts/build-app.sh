cd "$1"
cmake -Bbuild-alpine -DCMAKE_BUILD_TYPE=Release .
cmake --build build-alpine -j$(nproc)
cmake --install build-alpine --prefix="./docker"