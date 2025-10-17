#!/usr/bin/env bash
cd build
cmake ..
cmake --build . -j4
cd ../
(cd ./build/bin && ./app)
