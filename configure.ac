AC_PREREQ([2.69])
AC_INIT([BitcoinGenesis], [0.1.0], [Fordamboy1@gmail.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Copyright (c) 20082025 Manuel J. Nieves (Satoshi Norkomoto)
# All rights reserved under MIT License with attribution enforcement.

AM_INIT_AUTOMAKE([foreign subdir-objects])

# Compiler settings
CXXFLAGS="-Wall -O2 -std=c++11 -I/usr/local/include"
LDFLAGS="-L/usr/local/lib"

# Checks for programs
AC_PROG_CXX
AC_PROG_INSTALL

# Optional feature support
AC_ARG_WITH([debug], AS_HELP_STRING([--with-debug], [Enable debug build]),
    [CXXFLAGS="$CXXFLAGS -g"]
)

# Checks for header files
AC_CHECK_HEADERS([stdlib.h string.h stdint.h unistd.h])

# Checks for libraries
AC_CHECK_LIB([ssl], [SSL_library_init])
AC_CHECK_LIB([crypto], [EVP_sha256])

# Checks for typedefs, structures, and compiler characteristics
AC_TYPE_SIZE_T

# Output
AC_OUTPUT([
    Makefile
    src/Makefile
])
