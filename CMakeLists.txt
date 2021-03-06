#  Top-level CMakeLists.txt file for MDTK
#
#  Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2015 Oleksandr
#  Yermolenko <oleksandr.yermolenko@gmail.com>
#
#  This file is part of MDTK, the Molecular Dynamics Toolkit.
#
#  MDTK is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  MDTK is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with MDTK.  If not, see <http://www.gnu.org/licenses/>.
#

project (MDTK)

cmake_minimum_required(VERSION 2.4.5)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

IF (NOT MINGW_ROOT_PATH)
  SET(MINGW_ROOT_PATH "C:/msys/mingw")
ENDIF (NOT MINGW_ROOT_PATH)
IF (NOT EXTRA_ROOT_PATH)
  SET(EXTRA_ROOT_PATH "C:/msys/local")
ENDIF (NOT EXTRA_ROOT_PATH)

SET(CMAKE_FIND_ROOT_PATH  ${MINGW_ROOT_PATH} ${EXTRA_ROOT_PATH})

SET(MDTK_BINARY_SUFFIX "")

IF(WIN32)
  ADD_DEFINITIONS(-D__WIN32__)
ENDIF(WIN32)

IF(MDTK_HEAVY_OPTIMIZE)

  IF(BUILD_INTELCC)
    SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
    SET(MDTK_BINARY_SUFFIX "_intel")
    MESSAGE(STATUS "Using Intel C++ Compiler with optimizations!")
  ENDIF(BUILD_INTELCC)

  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-pedantic -W -Wall -Wno-long-long -O3")
    SET(CMAKE_C_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS_DISTRIBUTION}")
    MESSAGE(STATUS "Using GNU C++ Compiler with optimizations!")
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)

  SET(CMAKE_BUILD_TYPE distribution)

ELSE(MDTK_HEAVY_OPTIMIZE)

  SET(CMAKE_BUILD_TYPE Release)

ENDIF(MDTK_HEAVY_OPTIMIZE)

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

FUNCTION(CheckIfModuleExists Module)
  FIND_PACKAGE(${Module} QUIET)
  if(NOT DEFINED ${Module}_DIR)
    SET(MODULE_${Module}_EXISTS TRUE PARENT_SCOPE)
  ELSEIF(${Module}_DIR)
    SET(MODULE_${Module}_EXISTS TRUE PARENT_SCOPE)
  ELSE()
    SET(MODULE_${Module}_EXISTS FALSE PARENT_SCOPE)
  ENDIF()
ENDFUNCTION()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
FIND_PACKAGE(OSMesa)

FIND_PACKAGE(GSL REQUIRED)

SET(YAATK_COMPRESSION_INCLUDE_DIRS "")
SET(YAATK_COMPRESSION_LIBRARIES "")

FIND_PACKAGE(ZLIB)
IF(ZLIB_FOUND)
  SET(YAATK_COMPRESSION_INCLUDE_DIRS ${YAATK_COMPRESSION_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIR})
  SET(YAATK_COMPRESSION_LIBRARIES ${YAATK_COMPRESSION_LIBRARIES} ${ZLIB_LIBRARIES})
  add_definitions(-DYAATK_ENABLE_ZLIB)
ELSE(ZLIB_FOUND)
  MESSAGE(STATUS "ZLIB not found. YAATK will not be able to produce .gz files without external tools.")
ENDIF(ZLIB_FOUND)

CheckIfModuleExists(LibLZMA)
IF(MODULE_LibLZMA_EXISTS)
  FIND_PACKAGE(LibLZMA)
  IF(LIBLZMA_FOUND)
    SET(YAATK_COMPRESSION_INCLUDE_DIRS ${YAATK_COMPRESSION_INCLUDE_DIRS} ${LIBLZMA_INCLUDE_DIRS})
    SET(YAATK_COMPRESSION_LIBRARIES ${YAATK_COMPRESSION_LIBRARIES} ${LIBLZMA_LIBRARIES})
    # SET(YAATK_COMPRESSION_LIBRARIES ${YAATK_COMPRESSION_LIBRARIES} liblzma.a)
    add_definitions(-DYAATK_ENABLE_LIBLZMA)
  ELSE(LIBLZMA_FOUND)
    MESSAGE(STATUS "LibLZMA not found. YAATK will not be able to produce .xz files without external tools.")
  ENDIF(LIBLZMA_FOUND)
ELSE(MODULE_LibLZMA_EXISTS)
  MESSAGE(STATUS "CMake module for LibLZMA not found. Please consider updating CMake.")
ENDIF(MODULE_LibLZMA_EXISTS)

FIND_PACKAGE(PNG)

add_subdirectory (yaatk)
add_subdirectory (mdtk)
add_subdirectory (applications)
add_subdirectory (examples)

# may be useful for some old versions of Intel compilers
#  SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3 -D \"__sync_fetch_and_add(ptr,addend)=_InterlockedExchangeAdd(const_cast<void*>(reinterpret_cast<volatile void*>(ptr)), addend)\"")

