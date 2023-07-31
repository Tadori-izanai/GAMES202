# Install script for directory: /Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/lxl/Projects/cg/games202/hw2/prt/cmake-build-debug/ext_build/openexr/IlmBase/Imath/libImath.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathBoxAlgo.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathBox.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathColorAlgo.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathColor.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathEuler.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathExc.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathExport.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathForward.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathFrame.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathFrustum.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathFrustumTest.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathFun.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathGL.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathGLU.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathHalfLimits.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathInt64.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathInterval.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathLimits.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathLineAlgo.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathLine.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathMath.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathMatrixAlgo.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathMatrix.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathNamespace.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathPlane.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathPlatform.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathQuat.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathRandom.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathRoots.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathShear.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathSphere.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathVecAlgo.h"
    "/Users/lxl/Projects/cg/games202/hw2/prt/ext/openexr/IlmBase/Imath/ImathVec.h"
    )
endif()

