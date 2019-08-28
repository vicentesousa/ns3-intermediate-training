#!/bin/bash
#
# Build script for installing prerequisities of NS-3 installation on Ubuntu
#
echo Script construído por Vicente Sousa em 09/02/2017 para instalar o ns-3.27
echo Atualizado por Daniel Luna em 02/05/2019 para instalar o ns-3.29
#
# Dependencies installation
sudo apt-get update
sudo apt-get install multiarch-support liblzma5  zlib1g libc6 --fix-missing
sudo apt-get install gcc g++ python gcc g++ python python-dev mercurial python-setuptools git  qt5-default bzr gdb valgrind gsl-bin libgsl0-dev libgsl0ldbl bison libfl-dev tcpdump sqlite sqlite3 libsqlite3-dev libxml2 libxml2-dev libgtk2.0-0 libgtk2.0-dev vtun lxc uncrustify doxygen graphviz imagemagick texlive texlive-extra-utils texlive-latex-extra python-sphinx dia python-pygraphviz python-kiwi python-pygoocanvas libgoocanvas-dev libboost-signals-dev libboost-filesystem-dev wget

# gcc 4.9
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.9 g++-4.9
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9

# Eclipse CDT installation
sudo apt-get install eclipse eclipse-cdt g++

# ns-3 installation and compilation

# Criar pasta
mkdir ~/ns-3
cd ~/ns-3
# Baixar:
sudo wget https://www.nsnam.org/release/ns-allinone-3.29.tar.bz2 --no-check-certificate
#Descompactar:
tar xvjf ns-allinone-3.29.tar.bz2
#Entrar na pasta:
cd ns-allinone-3.29/ns-3.29/
# Configurar:
CXXFLAGS="-Wall -g -O0" ./waf --build-profile=debug --enable-examples --enable-tests configure
# Compilar:
./waf

# Testar 
./waf --run hello-simulator



echo For Eclipse configuration, see presentation: Tutorial_instalação_NS-3_e_integração_com_eclipsev06.pptx

