#!/bin/bash
#dependencies
#g++ | gcc > 4.9
#gumbo
[[ $(whoami) != 'root' ]] && echo "run as sudo!" && exit 1
echo "The install will update with apt-get update. continue(y/n)"
read yn
[[ "$yn" == 'n' ]] && exit
apt-get install aptitude
aptitude install libcurl-dev && sudo apt-get install libcurl4-gnutls-dev
apt-get install libtool
apt-get install mysql-client
apt-get install mysql-server
apt-get install libmysqlclient-dev
git clone git@github.com:google/gumbo-parser.git
#./autogen.sh
#./configure
#make
#make install
