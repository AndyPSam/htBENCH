#! /bin/zsh/

cd src
make
./htbench --hostname www.example.com
make clean
