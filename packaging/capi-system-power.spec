Name:       capi-system-power
Summary:    A power library in SLP C API
Version:    0.1.1
Release:    0
Group:      System/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001:     capi-system-power.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(pmapi)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(vconf)

%description
A power library in SLP C API package.

%package devel
Summary:  A power library in SLP C API (Development)
Group:    Development/System
Requires: %{name} = %{version}-%{release}

%description devel
A power library in SLP C API (Development) package.
%devel_desc

%prep
%setup -q

%build
cp %{SOURCE1001} .
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}
%__make %{?jobs:-j%jobs}

%install
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%license LICENSE.APLv2
%{_libdir}/libcapi-system-power.so.*

%files devel
%manifest %{name}.manifest
%{_includedir}/system/power.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-power.so
