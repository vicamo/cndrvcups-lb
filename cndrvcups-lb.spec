%define VERSION 2.40
%define README_VERSION 2.4x
%define  RELEASE 2

%ifarch x86_64
%define libs32  /usr/lib
%define locallibs  /usr/local/lib64
%else
%define libs32  %{_libdir}
%define locallibs      /usr/local/lib
%endif

Summary: Canon UFR II / LIPSLX Printer Driver for Linux v%{VERSION}
Name: cndrvcups-lb
Version: %{VERSION}
Release: %{RELEASE}
License: See the LICENSE*.txt file.
# Copyright: Copyright CANON INC. 2004-2011. All Rights Reserved.
Vendor: CANON INC.
Group: Applications/Publishing
Source0: cndrvcups-lb-%{version}-%{release}.tar.gz
BuildRoot: %{_tmppath}/%{name}-root
Requires: cndrvcups-common >= %{version}
BuildRequires: cndrvcups-common >= %{version}

%description 
Canon UFR II / LIPSLX Printer Driver for Linux. 
This UFR II / LIPSLX printer driver provides printing functions for Canon LBP/iR
printers operating under the CUPS (Common UNIX Printing System) environment.

%package -n cndrvcups-ufr2-uk
Summary: Canon UFR II Printer Driver for Linux v%{VERSION}
License: See the LICENSE*.txt file.
Vendor: CANON INC.
Group: Applications/Publishing
Requires: cndrvcups-common >= %{version}
BuildRequires: cndrvcups-common >= %{version}

%description -n cndrvcups-ufr2-uk
Canon UFR II Printer Driver for Linux. 
This UFR II printer driver provides printing functions for Canon LBP/iR
printers operating under the CUPS (Common UNIX Printing System) environment.

%package -n cndrvcups-ufr2-us
Summary: Canon UFR II Printer Driver for Linux v%{VERSION}
License: See the LICENSE*.txt file.
Vendor: CANON INC.
Group: Applications/Publishing
Requires: cndrvcups-common >= %{version}
BuildRequires: cndrvcups-common >= %{version}

%description -n cndrvcups-ufr2-us
Canon UFR II Printer Driver for Linux. 
This UFR II printer driver provides printing functions for Canon LBP/iR
printers operating under the CUPS (Common UNIX Printing System) environment.

%package -n cndrvcups-lipslx
Summary: Canon LIPSLX Printer Driver for Linux v%{VERSION}
License: See the LICENSE*.txt file.
Vendor: CANON INC.
Group: Applications/Publishing
Requires: cndrvcups-common >= %{version}
BuildRequires: cndrvcups-common >= %{version}

%description -n cndrvcups-lipslx
Canon LIPSLX Printer Driver for Linux. 
This LIPSLX printer driver provides printing functions for Canon LBP/iR
printers operating under the CUPS (Common UNIX Printing System) environment.


%prep

%setup -q

cd ppd
./autogen.sh --prefix=%{_prefix} 

cd ../pstoufr2cpca
./autogen.sh --prefix=%{_prefix} --libdir=%{_libdir}

cd ../backend
./autogen.sh --prefix=%{_prefix} --libdir=%{_libdir}
cd ..

cd cpca
./autogen.sh --prefix=%{_prefix} --enable-progpath=%{_bindir} --libdir=%{_libdir} --enable-static --disable-shared
cd ..

cd cngplp
./autogen.sh --libdir=%{locallibs}

cd files
./autogen.sh
cd ..

cd ..
 
%build
make 

%install

mkdir -p ${RPM_BUILD_ROOT}%{_bindir}
mkdir -p ${RPM_BUILD_ROOT}%{libs32}
mkdir -p ${RPM_BUILD_ROOT}%{_libdir}
mkdir -p ${RPM_BUILD_ROOT}%{_libdir}/cups/filter
mkdir -p ${RPM_BUILD_ROOT}%{_libdir}/cups/backend
mkdir -p ${RPM_BUILD_ROOT}%{_prefix}/share/cups/model
mkdir -p ${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
mkdir -p ${RPM_BUILD_ROOT}%{_prefix}/share/cnpkbidi
mkdir -p ${RPM_BUILD_ROOT}%{_prefix}/share/ufr2filter
mkdir -p ${RPM_BUILD_ROOT}%{_includedir}
# cngplp
mkdir -pv ${RPM_BUILD_ROOT}%{locallibs}
mkdir -pv ${RPM_BUILD_ROOT}%{_prefix}/share/cngplp

make install DESTDIR=${RPM_BUILD_ROOT}

install -c -m 4755 libs/cnpkmoduleufr2 	${RPM_BUILD_ROOT}%{_bindir}

install -c    -m 755 libs/libcanonufr2.la  ${RPM_BUILD_ROOT}%{libs32}
install -c -s -m 755 libs/libcanonufr2.so.1.0.0  ${RPM_BUILD_ROOT}%{libs32}
install -c -s -m 755 libs/libufr2filter.so.1.0.0   ${RPM_BUILD_ROOT}%{libs32}
install -c -s -m 755 libs/libEnoJBIG.so.1.0.0   ${RPM_BUILD_ROOT}%{libs32}
install -c -s -m 755 libs/cnpkbidi   ${RPM_BUILD_ROOT}%{_bindir}
install -c -s -m 755 libs/libcaiocnpkbidi.so.1.0.0   ${RPM_BUILD_ROOT}%{libs32}

install -c -m 644 data/CNC*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 data/CNL6*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 data/CNL8*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 data/CNL9*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 data/CNLC*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 data/CNLA*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 data/CNLB*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 data/CNLD*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 data/CnLB*	${RPM_BUILD_ROOT}%{_prefix}/share/caepcm
install -c -m 644 libs/cnpkbidi_info*	${RPM_BUILD_ROOT}%{_prefix}/share/cnpkbidi
install -c -m 644 libs/ThLB*	${RPM_BUILD_ROOT}%{_prefix}/share/ufr2filter

install -c -m 755 libs/libcnlbcm.so.1.0			${RPM_BUILD_ROOT}%{libs32}

cd ${RPM_BUILD_ROOT}%{libs32}
ln -sf libcanonufr2.so.1.0.0 libcanonufr2.so
ln -sf libcanonufr2.so.1.0.0 libcanonufr2.so.1
ln -sf libufr2filter.so.1.0.0 libufr2filter.so
ln -sf libufr2filter.so.1.0.0 libufr2filter.so.1
ln -sf libEnoJBIG.so.1.0.0 libEnoJBIG.so
ln -sf libEnoJBIG.so.1.0.0 libEnoJBIG.so.1
ln -sf libcaiocnpkbidi.so.1.0.0 libcaiocnpkbidi.so
ln -sf libcaiocnpkbidi.so.1.0.0 libcaiocnpkbidi.so.1
ln -sf libcnlbcm.so.1.0			libcnlbcm.so.1
ln -sf libcnlbcm.so.1.0			libcnlbcm.so
cd -

%clean
rm -rf $RPM_BUILD_ROOT

%post -n cndrvcups-ufr2-uk
if [ -d /usr/lib64/cups ]; then
	if [ -d /usr/lib/cups ]; then
		cd /usr/lib/cups/backend
		ln -sf ../../../lib64/cups/backend/cnusb cnusb
		cd /usr/lib/cups/filter
		ln -sf ../../../lib64/cups/filter/pstoufr2cpca pstoufr2cpca
	fi
fi
if [ -x /sbin/ldconfig ]; then
	/sbin/ldconfig
fi
%postun -n cndrvcups-ufr2-uk
if [ $1 = 0 ]; then
	if [ -d /usr/lib64/cups ]; then
		if [ -d /usr/lib/cups ]; then
			cd /usr/lib/cups/backend
			rm -f cnusb
			cd /usr/lib/cups/filter
			rm -f pstoufr2cpca
		fi
	fi
fi
if [ -x /sbin/ldconfig ]; then
	/sbin/ldconfig
fi

%post -n cndrvcups-ufr2-us
if [ -d /usr/lib64/cups ]; then
	if [ -d /usr/lib/cups ]; then
		cd /usr/lib/cups/backend
		ln -sf ../../../lib64/cups/backend/cnusb cnusb
		cd /usr/lib/cups/filter
		ln -sf ../../../lib64/cups/filter/pstoufr2cpca pstoufr2cpca
	fi
fi
if [ -x /sbin/ldconfig ]; then
	/sbin/ldconfig
fi
%postun -n cndrvcups-ufr2-us
if [ $1 = 0 ]; then
	if [ -d /usr/lib64/cups ]; then
		if [ -d /usr/lib/cups ]; then
			cd /usr/lib/cups/backend
			rm -f cnusb
			cd /usr/lib/cups/filter
			rm -f pstoufr2cpca
		fi
	fi
fi
if [ -x /sbin/ldconfig ]; then
	/sbin/ldconfig
fi

%post -n cndrvcups-lipslx
if [ -d /usr/lib64/cups ]; then
	if [ -d /usr/lib/cups ]; then
		cd /usr/lib/cups/backend
		ln -sf ../../../lib64/cups/backend/cnusb cnusb
		cd /usr/lib/cups/filter
		ln -sf ../../../lib64/cups/filter/pstoufr2cpca pstoufr2cpca
	fi
fi
if [ -x /sbin/ldconfig ]; then
	/sbin/ldconfig
fi
%postun -n cndrvcups-lipslx
if [ $1 = 0 ]; then
	if [ -d /usr/lib64/cups ]; then
		if [ -d /usr/lib/cups ]; then
			cd /usr/lib/cups/backend
			rm -f cnusb
			cd /usr/lib/cups/filter
			rm -f pstoufr2cpca
		fi
	fi
fi
if [ -x /sbin/ldconfig ]; then
	/sbin/ldconfig
fi

%files -n cndrvcups-ufr2-uk
%defattr(-,root,root)
%{libs32}/libcnlbcm.so*
%{libs32}/libcanonufr2.*
%{libs32}/libufr2filter.*
%{libs32}/libEnoJBIG.*
%{libs32}/libcaiocnpkbidi.*
%{_bindir}/cnpkbidi
%{_bindir}/cnpkmoduleufr2
%{_libdir}/libcnpkufr2.*
%{_libdir}/cups/filter/pstoufr2cpca
%{_libdir}/cups/backend/cnusb
%{_prefix}/share/cups/model/CNCUPS*ZK.ppd
%{_prefix}/share/caepcm/C*
%{_prefix}/share/cnpkbidi/cnpkbidi*
%{_prefix}/share/ufr2filter/ThLB*
%{_includedir}/cnpklibufr2.h
# cngplp
%{locallibs}/libuictlufr2.*
%{_prefix}/share/cngplp/cngplp_ufr2.glade
%{_prefix}/share/cngplp/CN*ZK.res
%{_prefix}/share/cngplp/common_ufr2.res
%{_prefix}/share/cngplp/func_config_ufr2.xml
%{_prefix}/local/share/locale/ja/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/fr/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/it/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/de/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/es/LC_MESSAGES/libuictlufr2*.mo

%doc README-ufr2-%{README_VERSION}UK.txt
%doc L*-ufr2-%{README_VERSION}*.txt
%doc LICENSE-ufr2-%{version}*.txt

%files -n cndrvcups-ufr2-us
%defattr(-,root,root)
%{libs32}/libcnlbcm.so*
%{libs32}/libcanonufr2.*
%{libs32}/libufr2filter.*
%{libs32}/libEnoJBIG.*
%{libs32}/libcaiocnpkbidi.*
%{_bindir}/cnpkbidi
%{_bindir}/cnpkmoduleufr2
%{_libdir}/libcnpkufr2.*
%{_libdir}/cups/filter/pstoufr2cpca
%{_libdir}/cups/backend/cnusb
%{_prefix}/share/cups/model/CNCUPS*ZS.ppd
%{_prefix}/share/caepcm/C*
%{_prefix}/share/cnpkbidi/cnpkbidi*
%{_prefix}/share/ufr2filter/ThLB*
%{_includedir}/cnpklibufr2.h
# cngplp
%{locallibs}/libuictlufr2.*
%{_prefix}/share/cngplp/cngplp_ufr2.glade
%{_prefix}/share/cngplp/CN*ZS.res
%{_prefix}/share/cngplp/common_ufr2.res
%{_prefix}/share/cngplp/func_config_ufr2.xml
%{_prefix}/local/share/locale/ja/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/fr/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/it/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/de/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/es/LC_MESSAGES/libuictlufr2*.mo

%doc README-ufr2-%{README_VERSION}US.txt
%doc LICENSE-ufr2-%{version}E.txt

%files -n cndrvcups-lipslx
%defattr(-,root,root)
%{libs32}/libcnlbcm.so*
%{libs32}/libcanonufr2.*
%{libs32}/libufr2filter.*
%{libs32}/libEnoJBIG.*
%{libs32}/libcaiocnpkbidi.*
%{_bindir}/cnpkbidi
%{_bindir}/cnpkmoduleufr2
%{_libdir}/libcnpkufr2.*
%{_libdir}/cups/filter/pstoufr2cpca
%{_libdir}/cups/backend/cnusb
%{_prefix}/share/cups/model/CNCUPS*ZJ.ppd
%{_prefix}/share/caepcm/C*
%{_prefix}/share/cnpkbidi/cnpkbidi*
%{_prefix}/share/ufr2filter/ThLB*
%{_includedir}/cnpklibufr2.h
# cngplp
%{locallibs}/libuictlufr2.*
%{_prefix}/share/cngplp/cngplp_ufr2.glade
%{_prefix}/share/cngplp/CN*ZJ.res
%{_prefix}/share/cngplp/common_ufr2.res
%{_prefix}/share/cngplp/func_config_ufr2.xml
%{_prefix}/local/share/locale/ja/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/fr/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/it/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/de/LC_MESSAGES/libuictlufr2*.mo
%{_prefix}/local/share/locale/es/LC_MESSAGES/libuictlufr2*.mo

%doc README-lipslx-%{README_VERSION}.txt
%doc LICENSE-lipslx-%{version}.txt
