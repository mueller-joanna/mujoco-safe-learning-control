#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/..

wget -qO- https://drake-apt.csail.mit.edu/drake.asc | gpg --dearmor - \
   | tee /etc/apt/trusted.gpg.d/drake.gpg >/dev/null 

echo "deb [arch=amd64] https://drake-apt.csail.mit.edu/$(lsb_release -cs) $(lsb_release -cs) main" \
   | tee /etc/apt/sources.list.d/drake.list >/dev/null 
   
apt-get update && apt-get install -y --no-install-recommends drake-dev
