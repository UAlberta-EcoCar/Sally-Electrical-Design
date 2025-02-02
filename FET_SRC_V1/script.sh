#!/usr/bin/zsh

echo "Configuring project tree for builds..."

rm -rf build && mkdir build && cmake -B build -G Ninja

echo "Call 'make' to build the project..."


