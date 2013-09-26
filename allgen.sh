_prefix=/usr
_bindir=/usr/bin

PARAMETER=$1

if [ `uname -m` != "x86_64" ]
then
	_locallibs=/usr/local/lib
else
	_locallibs=/usr/local/lib64
fi

cd ppd
./autogen.sh --prefix=${_prefix} 

cd ../pstoufr2cpca
./autogen.sh --prefix=${_prefix} --enable-progpath=${_bindir} --enable-shared --disable-static

cd ../backend
./autogen.sh --prefix=${_prefix}

cd ../cngplp
if [ $PARAMETER = "-deb" ]
then
	./autogen.sh --prefix=${_prefix} --libdir=/usr/lib
else
	./autogen.sh --libdir=${_locallibs}
fi

cd files
./autogen.sh
cd ..

cd ..
cd cpca/cnpklib
cd ..
./autogen.sh --prefix=${_prefix} --enable-static --disable-shared
make
make install
cd ..

make

