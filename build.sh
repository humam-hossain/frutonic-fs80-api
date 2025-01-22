set -xe

mkdir -p bin

g++ -Wall -Wextra -o bin/main src/main.cpp -lusb-1.0