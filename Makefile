CXX = g++								# compiler
CXXFLAGS = -g -Wall -MMD				# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = src/Number.o src/Matrix.o src/LP.o src/solver.o # object files forming executable
EXEC1 = Solver							# executable name

OBJECTS = ${OBJECTS1}
EXECS = ${EXEC1}
DEPENDS = ${OBJECTS:.o=.d}				# substitute ".o" with ".d"

.PHONY : all clean

all : ${EXECS}

${EXEC1} : ${OBJECTS1}					# link step
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}						# include *.d files containing program dependences

clean :									# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXECS}
