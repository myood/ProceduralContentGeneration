# TODO: instead of debian apt call, consider this: https://www.selectiveintellect.net/blog/2016/7/29/using-cmake-to-add-third-party-libraries-to-your-project-1

sudo apt install libsfml-dev libboost-all-dev
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
mkdir -p build && cd build && rm -rf * && cmake .. && make
