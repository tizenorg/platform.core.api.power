#sbs-git:slp/api/power capi-system-power 0.1.0 0432661af455f52366bdcd809dc72a00854c763c
Name:       capi-system-power
Summary:    A power library in Tizen C API
Version: 0.1.0
Release:    7
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
Source0:    %{name}-%{version}.tar.gz
Source1001: packaging/capi-system-power.manifest 
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(pmapi)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(vconf)

Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig

%description


%package devel
Summary:  A power library in Tizen C API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel



%prep
%setup -q


%build
cp %{SOURCE1001} .
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest capi-system-power.manifest
%{_libdir}/libcapi-system-power.so.*

%files devel
%manifest capi-system-power.manifest
%{_includedir}/system/power.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-power.so


