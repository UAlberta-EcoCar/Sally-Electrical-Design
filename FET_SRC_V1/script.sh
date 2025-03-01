#!/usr/bin/zsh

echo "Configuring project tree for builds..."

rm -rf build/Debug && mkdir build && mkdir build/Debug && cmake -B build/Debug -G Ninja

echo "Call 'make' to build the project..."


