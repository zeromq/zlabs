ACLOCAL_AMFLAGS = -I config

AM_CPPFLAGS = \
    -I$(srcdir)/include

EXTRA_DIST = \
	version.sh \
	builds/android/Android.mk \
	builds/android/Application.mk \
	builds/android/build.sh \
	builds/android/clean.sh \
	builds/mingw32/Makefile.mingw32 \
	builds/mingw32/platform.h \
	builds/msvc/build/buildbase.bat \
	builds/msvc/build/buildall.bat \
	builds/msvc/vs2012/zlabs.sln \
	builds/msvc/vs2012/zlabs_selftest/zlabs_selftest.props \
	builds/msvc/vs2012/zlabs_selftest/zlabs_selftest.vcxproj \
	builds/msvc/vs2012/zlabs/zlabs.props \
	builds/msvc/vs2012/zlabs/zlabs.vcxproj \
	builds/msvc/vs2012/zlabs/zlabs.vcxproj.filters \
	builds/msvc/vs2012/.gitignore \
	builds/msvc/vs2012/libzmq.import.props \
	builds/msvc/vs2012/libzmq.import.xml \
	builds/msvc/vs2012/zlabs.import.props \
	builds/msvc/vs2012/zlabs.import.xml \
	builds/msvc/vs2012/libsodium.import.props \
	builds/msvc/vs2012/libsodium.import.xml \
	builds/msvc/vs2008/README.txt \
	builds/msvc/vs2008/zlabs.sln \
	builds/msvc/vs2008/zlabs_selftest/zlabs_selftest.vcproj \
	builds/msvc/vs2008/zlabs/zlabs.vcproj \
	builds/msvc/.gitignore \
	builds/msvc/vs2010/zlabs.sln \
	builds/msvc/vs2010/zlabs_selftest/zlabs_selftest.props \
	builds/msvc/vs2010/zlabs_selftest/zlabs_selftest.vcxproj \
	builds/msvc/vs2010/zlabs/zlabs.props \
	builds/msvc/vs2010/zlabs/zlabs.vcxproj \
	builds/msvc/vs2010/zlabs/zlabs.vcxproj.filters \
	builds/msvc/vs2010/.gitignore \
	builds/msvc/vs2010/libzmq.import.props \
	builds/msvc/vs2010/libzmq.import.xml \
	builds/msvc/vs2010/zlabs.import.props \
	builds/msvc/vs2010/zlabs.import.xml \
	builds/msvc/vs2010/libsodium.import.props \
	builds/msvc/vs2010/libsodium.import.xml \
	builds/msvc/platform.h \
	builds/msvc/vs2013/zlabs.sln \
	builds/msvc/vs2013/zlabs_selftest/zlabs_selftest.props \
	builds/msvc/vs2013/zlabs_selftest/zlabs_selftest.vcxproj \
	builds/msvc/vs2013/zlabs/zlabs.props \
	builds/msvc/vs2013/zlabs/zlabs.vcxproj \
	builds/msvc/vs2013/zlabs/zlabs.vcxproj.filters \
	builds/msvc/vs2013/libzmq.import.props \
	builds/msvc/vs2013/libzmq.import.xml \
	builds/msvc/vs2013/zlabs.import.props \
	builds/msvc/vs2013/zlabs.import.xml \
	builds/msvc/vs2013/libsodium.import.props \
	builds/msvc/vs2013/libsodium.import.xml \
	builds/msvc/resource.h \
	builds/msvc/zlabs.rc \
	builds/msvc/properties/ReleaseLIB.props \
	builds/msvc/properties/Debug.props \
	builds/msvc/properties/LIB.props \
	builds/msvc/properties/ReleaseDLL.props \
	builds/msvc/properties/LTCG.props \
	builds/msvc/properties/Link.props \
	builds/msvc/properties/DebugLTCG.props \
	builds/msvc/properties/ReleaseLTCG.props \
	builds/msvc/properties/x64.props \
	builds/msvc/properties/Messages.props \
	builds/msvc/properties/DebugDEXE.props \
	builds/msvc/properties/Release.props \
	builds/msvc/properties/Win32.props \
	builds/msvc/properties/DLL.props \
	builds/msvc/properties/ReleaseSEXE.props \
	builds/msvc/properties/DebugSEXE.props \
	builds/msvc/properties/EXE.props \
	builds/msvc/properties/ReleaseDEXE.props \
	builds/msvc/properties/DebugLIB.props \
	builds/msvc/properties/Common.props \
	builds/msvc/properties/DebugDLL.props \
	builds/msvc/properties/ReleaseLEXE.props \
	builds/msvc/properties/DebugLEXE.props \
	builds/msvc/properties/Output.props \
	examples/zpubsub/protobuf/Makefile \
	examples/zpubsub/protobuf/participant.cc \
	examples/zpubsub/protobuf/participant.h \
	examples/zpubsub/protobuf/subscriber.cc \
	examples/zpubsub/protobuf/subscriber.h \
	examples/zpubsub/protobuf/testMessage.proto \
	examples/zpubsub/protobuf/testPublisher.cc \
	examples/zpubsub/protobuf/testSubscriber.cc \
	examples/zpubsub/protobuf/topic.cc \
	examples/zpubsub/protobuf/topic.h \
	examples/zpubsub/simple/Makefile \
	examples/zpubsub/simple/test_publisher.c \
	examples/zpubsub/simple/test_subscriber.c \
	examples/zpubsub/zprototest/include/zprototest_msg.h \
	examples/zpubsub/zprototest/src/Makefile \
	examples/zpubsub/zprototest/src/test_publisher.c \
	examples/zpubsub/zprototest/src/test_subscriber.c \
	examples/zpubsub/zprototest/src/zprototest_msg.c \
	examples/zpubsub/zprototest/src/zprototest_msg.xml \
	packaging/dist/gitlog2changelog.py \
	packaging/nuget/package.config \
	packaging/nuget/package.targets \
	packaging/nuget/package.bat \
	packaging/nuget/package.gsl \
	packaging/nuget/package.xml \
	packaging/nuget/package.nuspec

include $(srcdir)/model/Makefile.am.supplement

include $(srcdir)/src/Makefile.am.supplement

include $(srcdir)/doc/Makefile.am.supplement

# workaround the dist generated files that are also part of the distribution
#distcleancheck_listfiles = \
#	find . -type f -exec sh -c 'test -f $(srcdir)/{} || echo {}' ';'
# Note that distcleancheck is disabled for now, while waiting for a proper
# solution, that do not break older unix systems
distcleancheck:
	@:

# Automatically generate the ChangeLog from Git logs:
MAINTAINERCLEAN_FILES = ChangeLog
# Force ChangeLog regeneration upon make dist (due to nonexistant 'dummy-stamp'),
# in case it has already been generated previously
#
# Older boundary of the ChangeLog commits range
# It can be a tag ('v2.2.0'), a commit hash, a date, ...
# See gitrevisions for more information on specifying ranges
GIT_START_POINT=17707a584b
dummy-stamp:
ChangeLog: packaging/dist/gitlog2changelog.py dummy-stamp
	$(top_srcdir)/packaging/dist/gitlog2changelog.py $(GIT_START_POINT) || \
		echo "gitlog2changelog.py failed to generate the ChangeLog. See https://github.com/lovmoen/zlabs/commits/master" > $@
