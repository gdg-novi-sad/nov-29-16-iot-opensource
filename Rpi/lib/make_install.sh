#!/bin/bash


#  Defining base project directory and directories such as include, lib, etc.
BASE_DIR="$(readlink -f ../)"
echo $BASE_DIR

#  Defining temporary path
PATH_DIR="$BASE_DIR/lib/tmp"
#export PATH=$PATH_DIR/bin:$PATH

LIB_DIR="$PATH_DIR/lib"
INCLUDE_DIR="$PATH_DIR/include"

OUT_LIB="$BASE_DIR/out/lib"
OUT_INCLUDE="$BASE_DIR/out/include"

#  Re-touching necessary directories

rm -rf $PATH_DIR

mkdir $PATH_DIR


## libssl *
tar -xvzf openssl.tar.gz
cd openssl-OpenSSL_1_0_2i && ./config enable-egd enable-heartbeats enable-ssl3 		\
enable-ssl3-method enable-md2 enable-rc5 enable-rfc3779 enable-shared 	\
enable-ssl2 enable-weak-ssl-ciphers enable-zlib enable-zlib-dynamic 	\
--prefix=$PATH_DIR && cd ..
cd openssl-OpenSSL_1_0_2i && make && make install && cd ..


## paho.mqtt.c *
export LDFLAGS="-L$LIB_DIR"
export CFLAGS="-I$INCLUDE_DIR"
export CXXFLAGS="-I$INCLUDE_DIR"
export LD_LIBRARY_PATH=$LIB_DIR
export LIBRARY_PATH=$LIB_DIR


tar -xvzf paho.mqtt.c.tar.gz
cd paho.mqtt.c-1.1.0 && make && cd ..
cp -d paho.mqtt.c-1.1.0/build/output/* $LIB_DIR
rm -rf  $INCLUDE_DIR/mqtt
mkdir $INCLUDE_DIR/mqtt
cp -d paho.mqtt.c-1.1.0/src/MQTTAsync.h $INCLUDE_DIR/mqtt/
cp -d paho.mqtt.c-1.1.0/src/MQTTClient.h $INCLUDE_DIR/mqtt/
cp -d paho.mqtt.c-1.1.0/src/MQTTClientPersistence.h	 			\
$INCLUDE_DIR/mqtt/
cp -d paho.mqtt.c-1.1.0/src/MQTTAsync.h $INCLUDE_DIR
cp -d paho.mqtt.c-1.1.0/src/MQTTClient.h $INCLUDE_DIR
cp -d paho.mqtt.c-1.1.0/src/MQTTClientPersistence.h 				\
$INCLUDE_DIR
rm -f $LIB_DIR/MQTTVersion


unset LDFLAGS
unset CFLAGS
unset CXXFLAGS
unset LD_LIBRARY_PATH
unset LIBRARY_PATH


#

## paho.mqtt.cpp *
tar -xvzf org.eclipse.paho.mqtt.cpp.tar.gz
cd paho.mqtt.cpp-1.0 && make PAHO_C_HEADERS=$INCLUDE_DIR && cd ..
sed -i 									\
"s/\bMQTTAsync_connectOptions\b/public:MQTTAsync_connectOptions/g" 	\
paho.mqtt.cpp-1.0/src/mqtt/connect_options.h
cp -d paho.mqtt.cpp-1.0/lib/* $LIB_DIR
cp -d paho.mqtt.cpp-1.0/src/mqtt/* $INCLUDE_DIR/mqtt

cp -r $LIB_DIR $OUT_LIB
cp -r $INCLUDE_DIR $OUT_INCLUDE


cd poco-1.7.5-all
./configure --include-path=$OUT_INCLUDE --library-path=$OUT_LIB --no-tests --no-samples  --omit=Data/ODBC,Data/MySQL --prefix=_INSTALL
make
make install
cp -rf _INSTALL/lib/* $OUT_LIB
cp -rf _INSTALL/include/* $OUT_INCLUDE
cd ..


rm -rf $PATH_DIR
rm -rf openssl-OpenSSL_1_0_2i
rm -rf paho.mqtt.c-1.1.0
rm -rf paho.mqtt.cpp-1.0
#rm -rf poco-1.7.5-all
rm -rf $OUT_LIB/pkgconfig





