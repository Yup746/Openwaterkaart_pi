##---------------------------------------------------------------------------
## Author:      Pim Spithost
## Copyright:
## License:     
##---------------------------------------------------------------------------
## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##
##

##
## ----- Modify section below to include all the details for your plugin ----- ##
##

set(CMLOC "CMakeLists: ")

# define minimum cmake version
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

# define plugin name, owner and versions

SET(VERBOSE_NAME "Openwaterkaart_pi")
SET(COMMON_NAME "Openwaterkaart")
SET(TITLE_NAME "Openwaterkaart")
SET(PACKAGE_CONTACT "Pim Spithost")
SET(PACKAGE "openwaterkaart")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")

set(VERSION_PATCH "0")
set(VERSION_TWEAK "0")
set(VERSION_DATE "24/10/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")
#
#
#
#
#
#
#
# The next line allows for setting up your own local webserver with git on it for testing purposes, the default is github.com
set(GIT_REPOSITORY_SERVER "github.com")

# Specifies cloudsmith repository sufficies
set(PROD "prod") # Standard Repos
set(BETA "beta") # Standard Repos
set(ALPHA "alpha") # Standard Repos

set(CLOUDSMITH_USER "opencpn")
# set this if your cloudsmith repository name does not match your git repository name set(CLOUDSMITH_BASE_REPOSITORY "sar")
set(CLOUDSMITH_BASE_REPOSITORY "dr")

set(XML_INFO_URL "https://www.openwaterkaart.nl")
set(XML_SUMMARY "Plugin for charts and information from Openwaterkaart")
set(XML_DESCRIPTION "The Openwaterkaart plugin allows for downloading and integrating Openwaterkaart water charts as well as helpfull information")

##
## ----- Modify section above to include all the details for your plugin ----- ##
##
##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

OPTION(DR_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
###set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
###set(USE_LOCAL_GLU FALSE)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

##
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##
project(${PACKAGE})

include("cmake/PluginSetup.cmake")

##
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE ON)
option(Plugin_CXX11 "Use c++11" OFF)

##
## ----- Modify section above if there are special requirements for the plugin ----- ##
##

##
## ----- do not change next section - needed to configure build process ----- ##
##

include("cmake/PluginConfigure.cmake")

##
## ----- do not change section above - needed to configure build process ----- ##
##

#
# ----- Change below to match project requirements for source, headers, etc. ----- ##
#

SET(SRC_DR
        src/Openwaterkaart_pi.h
        src/Openwaterkaart_pi.cpp
        src/icons.h
        src/icons.cpp
	    src/Openwaterkaartgui.h
	    src/Openwaterkaartgui.cpp
	    src/Openwaterkaartgui_impl.cpp
	    src/Openwaterkaartgui_impl.h
		src/ocpn_plugin.h
		src/tinyxml2.cpp
		src/tinyxml2.h
	)


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_DR})

ADD_DEFINITIONS(-DDR_USE_SVG)
ADD_DEFINITIONS(-DTIXML_USE_STL)

##
## ----- Change above to match project requirements ----- ##
##

##
## ----- do not change next section - needed to configure build process ----- ##
##

include("cmake/PluginInstall.cmake")
include("cmake/PluginLocalization.cmake")
include("cmake/PluginPackage.cmake")

##
## ----- do not change section above - needed to configure build process ----- ##
##
