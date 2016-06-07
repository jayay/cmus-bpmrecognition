#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cmus-bpmrecognition], [1.0.0], [jakob-jungmann@hotmail.de])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_HEADERS([include/])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lSoundTouch':
AC_CHECK_LIB([aubio], [inputSamples])
# FIXME: Replace `main' with a function in `-lc':
AC_CHECK_LIB([c], [main])
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stddef.h stdint.h stdlib.h string.h strings.h sys/time.h unistd.h wchar.h wctype.h])

AC_PREFIX_PROGRAM(cmus)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRCOLL
AC_FUNC_STRTOD
AC_CHECK_FUNCS([dup2 getcwd gettimeofday memset mkdir munmap pow select strcasecmp strchr strdup strerror strndup strrchr strstr strtol])
AC_DEFINE_UNQUOTED([LIBDIR], ["$prefix/lib"], [lib path of cmus input plugins.])

# AC_CONFIG_FILES([Makefile])
AC_OUTPUT
