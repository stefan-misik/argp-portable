#!/bin/sh
set -x

make

./argp-test -a"abc def"
./argp-test -a "abc def"
./argp-test -iaabc

./argp-test -b --info --ab=abc
./argp-test --ba --ababc
./argp-test --ab= -i --info 
