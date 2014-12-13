#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin_4.x-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AugmentedNeighbourhoodCDHS_GMH.o \
	${OBJECTDIR}/CDHS_GMH.o \
	${OBJECTDIR}/Chaos_GMH.o \
	${OBJECTDIR}/Degrees_GMH.o \
	${OBJECTDIR}/Edge.o \
	${OBJECTDIR}/FileMetadata.o \
	${OBJECTDIR}/Graph.o \
	${OBJECTDIR}/GroupMappingHeuristic.o \
	${OBJECTDIR}/InputManagement.o \
	${OBJECTDIR}/MHESonCore.o \
	${OBJECTDIR}/ParametrizationSettings.o \
	${OBJECTDIR}/Protocol.o \
	${OBJECTDIR}/RandomNumberGenerator.o \
	${OBJECTDIR}/Vertex.o \
	${OBJECTDIR}/VertexGroup.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11 -std=gnu++11 -std=c++0x -O3
CXXFLAGS=-std=c++11 -std=gnu++11 -std=c++0x -O3

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mheson-sn.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mheson-sn.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mheson-sn ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/AugmentedNeighbourhoodCDHS_GMH.o: AugmentedNeighbourhoodCDHS_GMH.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/AugmentedNeighbourhoodCDHS_GMH.o AugmentedNeighbourhoodCDHS_GMH.cpp

${OBJECTDIR}/CDHS_GMH.o: CDHS_GMH.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CDHS_GMH.o CDHS_GMH.cpp

${OBJECTDIR}/Chaos_GMH.o: Chaos_GMH.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Chaos_GMH.o Chaos_GMH.cpp

${OBJECTDIR}/Degrees_GMH.o: Degrees_GMH.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Degrees_GMH.o Degrees_GMH.cpp

${OBJECTDIR}/Edge.o: Edge.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Edge.o Edge.cpp

${OBJECTDIR}/FileMetadata.o: FileMetadata.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/FileMetadata.o FileMetadata.cpp

${OBJECTDIR}/Graph.o: Graph.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Graph.o Graph.cpp

${OBJECTDIR}/GroupMappingHeuristic.o: GroupMappingHeuristic.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/GroupMappingHeuristic.o GroupMappingHeuristic.cpp

${OBJECTDIR}/InputManagement.o: InputManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/InputManagement.o InputManagement.cpp

${OBJECTDIR}/MHESonCore.o: MHESonCore.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/MHESonCore.o MHESonCore.cpp

${OBJECTDIR}/ParametrizationSettings.o: ParametrizationSettings.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ParametrizationSettings.o ParametrizationSettings.cpp

${OBJECTDIR}/Protocol.o: Protocol.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Protocol.o Protocol.cpp

${OBJECTDIR}/RandomNumberGenerator.o: RandomNumberGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RandomNumberGenerator.o RandomNumberGenerator.cpp

${OBJECTDIR}/Vertex.o: Vertex.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Vertex.o Vertex.cpp

${OBJECTDIR}/VertexGroup.o: VertexGroup.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/VertexGroup.o VertexGroup.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -std=c++11 -std=gnu++11 -std=c++0x -O3 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mheson-sn.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
