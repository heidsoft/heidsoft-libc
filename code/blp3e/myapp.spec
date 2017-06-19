#
# spec file for package myapp (Version 1.0)
#

Vendor:           Wrox Press
Distribution:     Any
Name:	             myapp
Release:          1
Packager:         neil@provider.com
Copyright:        2003 by Wrox Press
Group:            Applications/Media
Provides:         myapp
Requires:         mysql-3.23
Autoreqprov:      on
Version:          1.0
Summary:          Trivial application
Source:           myapp-1.0.tar.gz

%description
MyAPP Trivial Application

A trivial application used to demonstrate development tools.

This version pretends it requires MySQL 3.23.

Authors: Neil Matthew and Richard Stones

%prep
%setup

%build
make

%install
make install

%files
/usr/local/bin/myapp
/usr/local/man/man1/myapp.1.gz
