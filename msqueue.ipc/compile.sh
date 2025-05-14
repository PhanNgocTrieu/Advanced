# This script compiles the project using cmake and make
cmake_path="${HOME}/software/cmake-3.17.0-rc3/bin/cmake"
if [ ! -f "${cmake_path}" ]; then
    echo "cmake not found at ${cmake_path}"
    exit 1
fi

# Build the project
if [ ! -f "${PWD}/build" ]; then
    mkdir -p build
fi
cd build
"${cmake_path}" ..
make
