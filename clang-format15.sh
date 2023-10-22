#! /bin/bash

cd /tmp
wget https://apt.llvm.org/llvm.sh
sudo apt install python3-lldb-15
sudo bash llvm.sh 15
sudo apt install clang-format-15
cd /usr/bin
rm -rf clang-format
sudo ln -s clang-format-15 clang-format
