#!/usr/bin/env bash
cd build
cmake ..
cmake --build .
cd ../
(cd ./build/bin && ./app)
