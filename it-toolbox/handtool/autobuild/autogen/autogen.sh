#!/bin/bash
echo 
echo ... auto_test autogen ...
echo 

## check all dependencies are present
MISSING=""

## check for aclocal
env aclocal --version > /dev/null 2>&1
if [ $? -eq 0 ]; then
	ACLOCAL=aclocal
else
	MISSING="$MISSING aclocal"
fi

## check for autoconf
env autoconf --version > /dev/null 2>&1
if [ $? -eq 0 ]; then 
	AUTOCONF=autoconf
else
	MISSING="$MISSING autoconf"
fi

## check for autoheader
env autoheader --version > /dev/null 2>&1
if [ $? -eq 0 ]; then 
	AUTOHEADER=autoheader
else
	MISSING="$MISSING autoheader"
fi

## check for automake
env automake --version > /dev/null 2>&1
if [ $? -eq 0 ]; then 
	AUTOMAKE=automake
else
	MISSING="$MISSING automake"
fi

## check for libtoolize or glibtoolize
env libtoolize --version > /dev/null 2>&1
if [ $? -eq 0 ]; then 
	TOOL=libtoolize
else
	env glibtoolize --version > /dev/null 2>&1
	if [ $? -eq 0 ]; then 
		TOOL=glibtoolize
	else
		MISSING="$MISSING libtoolize/glibtoolize"
	fi
fi

## check for tar
env tar -cf /dev/null /dev/null  > /dev/null 2>&1
if [ $? -ne 0 ]; then 
	MISSING="$MISSING tar"
fi

## if dependencies are missing, warn the user and abort
if [ "x$MISSING" != "x" ]; then
	echo "Aborting."
	echo 
	echo "The following build tools are missing:"
	echo
	for pkg in $MISSING; do
		echo "  * $pkg"
	done
	echo 
	echo "Please install then and try again"
	ehco 
	exit 1
fi

## do the autogeneration
echo Running ${ACLOCAL}...
$ACLOCAL
echo Running ${AUTOHEADER}...
$AUTOHEADER
echo Running ${TOOL}...
$TOOL --automake --copy --force
echo Running ${AUTOCONF}...
$AUTOCONF
echo Running ${AUTOMAKE}...
$AUTOMAKE --add-missing --force-missing --copy --foreign

## run autogen in the argp-standalone sub-directory
#echo "running autogen.sh in argp-standalone ..."
#( cd contrib/argp-standalone; ./autogen.sh)

## instruct user on next steps
echo 
echo "Please proceed with configuring, compiling and installing."
