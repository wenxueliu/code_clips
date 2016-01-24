# Copyright (c) 2013 Luca Barbato
# Copyright (c) 2013 GoPivotal, Inc.  All rights reserved.
# Copyright 2015 Garrett D'Amore <garrett@damore.org>
# Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 Nicira, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom
# the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

################################################################################
#  Start the configuration phase.                                              #
################################################################################

#aclocal;autoconf configure.ac > configure; ./configure

AC_PREREQ([2.63])

AC_INIT([zlib], [version-0.1])

echo "======================== RUN ac_config_macro_dir m4 ========================"
AC_CONFIG_MACRO_DIR([m4])

################################################################################
#  Check the compilers.                                                         #
################################################################################

echo "======================== RUN ac_prog_cc     ========================"
#AC_PROG_CC(clang, gcc)
echo "======================== RUN ac_prog_cc_c99 ========================"
AC_PROG_CC_C99
AC_MSG_NOTICE([ac_cv_prog_cc_c99=$ac_prog_cc_c99])
echo "======================== RUN ac_prog_cc_c_o ========================"
#AC_PROG_CC_C_O
AC_MSG_NOTICE([ ac_cv_prog_cc_gcc_c_o=$ac_cv_prog_cc_gcc_c_o ])
AC_MSG_NOTICE([ ac_cv_prog_cc_clang_c_o=$ac_cv_prog_cc_clang_c_o ])
echo "======================== RUN ac_prog_cc_cpp ========================"
#AC_PROG_CPP
AC_MSG_NOTICE([ CPP=$CPP ])
echo "======================== RUN ac_prog_mkdir_p ========================"
AC_PROG_MKDIR_P
echo "======================== RUN ac_prog_fgrep ========================"
AC_PROG_FGREP
echo "======================== RUN ac_prog_egrep ========================"
AC_PROG_EGREP

echo "======================== check perl environment ========================"
AC_MSG_NOTICE([ ac_cv_path_perl=$ac_cv_path_perl ])
AC_MSG_NOTICE([ PERL=$PERL ])
AC_ARG_VAR([PERL], [path to Perl interpreter])
echo "======================== RUN ac_path_prog([perl], perl, no) ========================"
AC_PATH_PROG([PERL], perl, no)
if test "$PERL" = no; then
   AC_MSG_ERROR([Perl interpreter not found in $PATH or $PERL.])
fi
AC_PATH_PROG([PERL], perl, no)
AC_MSG_NOTICE([ ac_cv_path_perl=$ac_cv_path_perl ])
AC_MSG_NOTICE([ PERL=$PERL ])


echo "======================== RUN am_missing_prog ========================"
AM_MISSING_PROG([AUTOM4TE], [autom4te])

echo "======================== RUN ac_use_system_extensions ========================"
AC_USE_SYSTEM_EXTENSIONS
echo "======================== RUN ac_c_bigendian ========================"
AC_C_BIGENDIAN
echo "======================== RUN ac_sys_largefile ========================"
AC_SYS_LARGEFILE

echo "======================== RUN disable-shared========================"
#LT_INIT([disable-shared])
#m4_pattern_forbid([LT_INIT]) dnl Make autoconf fail if libtool is missing.

echo "======================== RUN ac_search_libs pow m========================"
AC_SEARCH_LIBS([pow], [m])
AC_MSG_NOTICE([ac_cv_search_pow=$ac_cv_search_pow])

echo "======================== RUN ac_search_libs clock_gettime rt========================"
AC_SEARCH_LIBS([clock_gettime], [rt])
AC_MSG_NOTICE([ac_cv_search_clock_gettime=$ac_cv_search_clock_gettime])

echo "======================== RUN ac_search_libs timer_create rt========================"
AC_SEARCH_LIBS([timer_create], [rt])
AC_MSG_NOTICE([ac_cv_search_timer_create=$ac_cv_search_timer_create])

echo "======================== RUN ac_search_libs timer_create rt========================"
AC_SEARCH_LIBS([pthread_create], [pthread])
AC_MSG_NOTICE([ac_cv_search_pthread_create=$ac_cv_search_pthread_create])

echo "======================== RUN ac_search_libs socketpair ========================"
AC_SEARCH_LIBS([socketpair], [], [
    AC_DEFINE([_HAVE_SOCKETPAIR])
])
AC_MSG_NOTICE([ac_cv_search_socket_pair=$ac_cv_search_socket_pair])

echo "======================== RUN ac_search_libs sem_wait rt pthread========================"
AC_SEARCH_LIBS([sem_wait], [rt pthread], [
    AC_DEFINE([_HAVE_SEMAPHORE])
])
AC_MSG_NOTICE([ac_cv_search_sem_wait=$ac_cv_search_sem_wait])

echo "======================== RUN ac_func_strerror_r ========================"
AC_FUNC_STRERROR_R
AC_MSG_NOTICE([HAVE_STRERROR_R=$HAVE_STRERROR_R])
AC_MSG_NOTICE([HAVE_DECL_STRERROR_R=$HAVE_DECL_STRERROR_R])
AC_MSG_NOTICE([STRERROR_R_CHAR_P=$STRERROR_R_CHAR_P])


echo "======================== RUN ovs_check_esx ========================"
AC_MSG_NOTICE([ESX=$ESX])
AC_MSG_NOTICE([ac_cv_header_vmware=$ac_cv_header_vmware])

echo "======================== RUN ovs_check_win32 ========================"
OVS_CHECK_WIN32
AC_MSG_NOTICE([WIN32=$WIN32])

echo "======================== RUN ovs_check_visual_studio_ddk ========================"
OVS_CHECK_VISUAL_STUDIO_DDK
AC_MSG_NOTICE([VSTUDIO_DDK=$VSTUDIO_DDK])
AC_MSG_NOTICE([VSTUDIO_CONFIG=$VSTUDIO_CONFIG])

echo "======================== RUN ovs_check_coverage ========================"
AC_MSG_NOTICE([CFLAGS=$CFLAGS])
OVS_CHECK_COVERAGE
AC_MSG_NOTICE([CFLAGS=$CFLAGS])

echo "======================== RUN ovs_check_ndebug ========================"
OVS_CHECK_NDEBUG
AC_MSG_NOTICE([NDEBUG=$NDEBUG])

echo "======================== RUN ovs_check_netlink ========================"
OVS_CHECK_NETLINK
AC_MSG_NOTICE([HAVE_NETLINK=$HAVE_NETLINK])

echo "======================== RUN ovs_check_openssl ========================"
OVS_CHECK_OPENSSL
AC_MSG_NOTICE([HAVE_OPENSSL=$HAVE_OPENSSL])

echo "======================== RUN ovs_check_logdir ========================"
OVS_CHECK_LOGDIR
AC_MSG_NOTICE([PACKAGE=$PACKAGE])
AC_MSG_NOTICE([localstatedir=$localstatedir])
AC_MSG_NOTICE([LOGDIR=$LOGDIR])

echo "======================== RUN ovs_check_python ========================"
AC_MSG_NOTICE([PYTHON=$PYTHON])
AC_MSG_NOTICE([PATN_SEPARATOR=$PATH_SEPARATOR])
OVS_CHECK_PYTHON
AC_MSG_NOTICE([HAVE_PYTHON=$HAVE_PYTHON])

echo "======================== RUN ovs_check_dot ========================"
OVS_CHECK_DOT
AC_MSG_NOTICE([HAVE_DOT=$HAVE_DOT])

echo "======================== RUN ovs_check_if_packet ========================"
OVS_CHECK_IF_PACKET
AC_MSG_NOTICE([HAVE_IF_PACKET=$HAVE_IF_PACKET])

echo "======================== RUN ovs_check_if_dl ========================"
OVS_CHECK_IF_DL
AC_MSG_NOTICE([HAVE_IF_DL=$HAVE_IF_DL])

echo "======================== RUN ovs_check_strtok_r ========================"
OVS_CHECK_STRTOK_R
AC_MSG_NOTICE([HAVE_STRTOK_R_BUG=$HAVE_STRERROR_R])

echo "======================== RUN ac_check_decls sys_siglist ========================"
AC_CHECK_DECLS([sys_siglist], [], [], [[#include <signal.h>]])

AC_MSG_NOTICE([HAVE_DECL_SYS_SIGLIST=$HAVE_DECL_SYS_SIGLIST])
AC_MSG_NOTICE([HAVE_DECL_sys_siglist=$HAVE_DECL_sys_siglist])
AC_MSG_NOTICE([ac_cv_have_decl_sys_siglist=$ac_cv_have_decl_sys_siglist])


echo "======================== RUN ac_check_member stat ========================"
AC_CHECK_MEMBERS([struct stat.st_mtim.tv_nsec, struct stat.st_mtimensec],
  [], [], [[#include <sys/stat.h>]])
AC_MSG_NOTICE([ac_cv_member_stat_st_mtim_tv_nsec=$ac_cv_member_stat_st_mtim_tv_nsec])
AC_MSG_NOTICE(HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC=$HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC)
AC_MSG_NOTICE(HAVE_STRUCT_STAT_ST_MTIMENSEC=$HAVE_STRUCT_STAT_ST_MTIMENSEC)

echo "======================== RUN ac_check_member ifreq ========================"
AC_CHECK_MEMBERS([struct ifreq.ifr_flagshigh], [], [], [[#include <net/if.h>]])

echo "======================== RUN ac_check_funcs ========================"
AC_CHECK_FUNCS([mlockall strnlen getloadavg statvfs getmntent_r])
AC_MSG_NOTICE(HAVE_MLOCKALL=$HAVE_MLOCKALL)
AC_MSG_NOTICE(HAVE_STRNLEN=$HAVE_STRNLEN)
AC_MSG_NOTICE(HAVE_GETLOADAVG=$HAVE_GETLOADAVG)
AC_MSG_NOTICE(HAVE_STATVFS=$HAVE_STATVFS)
AC_MSG_NOTICE(HAVE_GETMNTENT_R=$HAVE_GETMNTENT_R )
#AC_MSG_NOTICE(AC_CHECK_FUNC=$AC_CHECK_FUNC)

echo "======================== RUN ac_check_headers ========================"
AC_CHECK_HEADERS([mntent.h sys/statvfs.h linux/types.h linux/if_ether.h stdatomic.h])
AC_CHECK_HEADERS([net/if_mib.h], [], [], [[#include <sys/types.h>
#include <net/if.h>]])
AC_MSG_NOTICE(HAVE_MNTENT_H=$HAVE_MNTENT_H)
AC_MSG_NOTICE(HAVE_SYS_STATVFS_H=$HAVE_SYS_STATVFS_H)
AC_MSG_NOTICE(HAVE_LINUX_TYPES_H=$HAVE_LINUX_TYPES_H)
AC_MSG_NOTICE(HAVE_LINUX_IF_ETHER_H=$HAVE_LINUX_IF_ETHER_H)
AC_MSG_NOTICE(HAVE_LINUX_STDATOMIC_H=$HAVE_LINUX_STDATOMIC_H)
AC_MSG_NOTICE(HAVE_NET_IF_MIB_H=$HAVE_NET_IF_MIB_H)

echo "======================== RUN ovs_check_backtrace ========================"
OVS_CHECK_BACKTRACE

echo "======================== RUN ovs_check_per_event ========================"
OVS_CHECK_PERF_EVENT
AC_MSG_NOTICE(HAVE_LINUX_PERF_EVENT_H=$HAVE_NET_IF_MIB_H)

echo "======================== RUN ovs_check_per_event ========================"
OVS_CHECK_VALGRIND
AC_MSG_NOTICE(HAVE_VALGRIND_VALGRIND_H=$HAVE_VALGRIND_VALGRIND_H)

echo "======================== RUN ovs_check_socket_libs ========================"
OVS_CHECK_SOCKET_LIBS

echo "======================== RUN ovs_check_groff ========================"
OVS_CHECK_GROFF

echo "======================== RUN ovs_check_gun_make ========================"
OVS_CHECK_GNU_MAKE
AC_MSG_NOTICE(SPARSEFLAGS=$SPARSEFLAGS)
AC_MSG_NOTICE(CGCCFLAGS=$CGCCFLAGS)

echo "======================== RUN ovs_check_tls ========================"
OVS_CHECK_TLS
AC_MSG_NOTICE(ovs_cv_thread_local=$ovs_cv_thread_local)
AC_MSG_NOTICE(ovs_cv___thread=$ovs_cv___thread)

echo "======================== RUN ovs_check_atomic_libs_========================"
OVS_CHECK_ATOMIC_LIBS


echo "======================== RUN ovs_check_gcc4_atomics ========================"
OVS_CHECK_GCC4_ATOMICS
AC_MSG_NOTICE(ovs_cv_gcc4_atomics=$ovs_cv_gcc4_atomics)

echo "======================== RUN ovs_check_atomic_always_lock_free ========================"
OVS_CHECK_ATOMIC_ALWAYS_LOCK_FREE(1)
OVS_CHECK_ATOMIC_ALWAYS_LOCK_FREE(2)
OVS_CHECK_ATOMIC_ALWAYS_LOCK_FREE(4)
OVS_CHECK_ATOMIC_ALWAYS_LOCK_FREE(8)

echo "======================== RUN ovs_check_posix_aio ========================"
OVS_CHECK_POSIX_AIO


echo "======================== RUN ovs_check_pthread_set_name ========================"
OVS_CHECK_PTHREAD_SET_NAME

echo "======================== RUN ovs_check_linux_host ======================== "
OVS_CHECK_LINUX_HOST

echo "======================== RUN ax_func_posix_memalign ========================"
AX_FUNC_POSIX_MEMALIGN


echo "======================== RUN ovs_check_include_next ========================"
OVS_CHECK_INCLUDE_NEXT

#AC_CONFIG_FILES()

echo "======================== RUN ovs_enable_option ========================"
OVS_ENABLE_OPTION([-Wall])
OVS_ENABLE_OPTION([-Wextra])
OVS_ENABLE_OPTION([-Wno-sign-compare])
OVS_ENABLE_OPTION([-Wpointer-arith])
OVS_ENABLE_OPTION([-Wformat-security])
OVS_ENABLE_OPTION([-Wswitch-enum])
OVS_ENABLE_OPTION([-Wunused-parameter])
OVS_ENABLE_OPTION([-Wbad-function-cast])
OVS_ENABLE_OPTION([-Wcast-align])
OVS_ENABLE_OPTION([-Wstrict-prototypes])
OVS_ENABLE_OPTION([-Wold-style-definition])
OVS_ENABLE_OPTION([-Wmissing-prototypes])
OVS_ENABLE_OPTION([-Wmissing-field-initializers])
OVS_ENABLE_OPTION([-Wthread-safety])
OVS_ENABLE_OPTION([-fno-strict-aliasing])
OVS_ENABLE_OPTION([-Qunused-arguments])
OVS_CONDITIONAL_CC_OPTION([-Wno-unused], [HAVE_WNO_UNUSED])
OVS_CONDITIONAL_CC_OPTION([-Wno-unused-parameter], [HAVE_WNO_UNUSED_PARAMETER])

OVS_ENABLE_WERROR
OVS_ENABLE_SPARSE

AC_ARG_VAR(KARCH, [Kernel Architecture String])
AC_SUBST(KARCH)
OVS_CHECK_LINUX
OVS_CHECK_DPDK
OVS_CHECK_PRAGMA_MESSAGE
AC_SUBST([OVS_CFLAGS])
AC_SUBST([OVS_LDFLAGS])

AC_CONFIG_FILES(Makefile)

#AC_MSG_NOTICE([AC_INCLUDES_DEFAULT])
#TESTB
#TESTA
#TESTA
#TESTB
#TESTC
#OUTER

AC_OUTPUT
