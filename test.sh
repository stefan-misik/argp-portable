#!/bin/sh
set -x

make

./argp-test -t"abc def"
./argp-test -t "abc def"
./argp-test -itabc

./argp-test -b --info --test=abc
./argp-test --ts --abbabc
./argp-test --test= -i --info 
