#!/bin/sh
echo "begin install"
if ! [ -x "$(command -v cmake)" ]; then
  echo 'need be root to begin install cmake,give password' 
  sudo yum install cmake
fi

if ! [ -x "$(command -v )" ]; then
  echo 'need be root to begin install fftw3,give password,if is root negative' 
  sudo yum install fftw3 
fi

if ! [ -x "$(command -v )" ]; then
  echo 'need be root to begin install gsl,give password,if is root negative' 
  sudo yum install gsl 
fi

if [ ! -d "./build" ];then
  mkdir build
fi

cd build
