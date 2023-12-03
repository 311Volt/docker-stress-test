wget -O crow.tar.gz https://github.com/CrowCpp/Crow/archive/refs/tags/v1.0+5.tar.gz
mkdir crow
tar -xzvf crow.tar.gz -C crow
ls -la
cd crow/Crow-1.0-5/
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release -DCROW_BUILD_TESTS=OFF -DCROW_BUILD_EXAMPLES=OFF .
cmake --install build