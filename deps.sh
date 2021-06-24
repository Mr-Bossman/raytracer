#!/bin/bash 
sudo apt update
sudo apt install libtinfo-dev libgmp-dev git wget build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
#sudo apt install nvidia-cuda-toolkit
#sudo apt install opencl-headers ocl-icd-opencl-dev
#sry arch and red hat users
wget -qO- https://get.haskellstack.org/ | sh 
#oh god security
git clone https://github.com/diku-dk/futhark.git
cd futhark
stack setup
stack build
stack install
git submodule init
export PATH=$PATH:~/.local/bin
cd src/futhark
futhark pkg sync