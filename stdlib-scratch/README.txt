
	Code for The C++ Standard Library from Scratch


Introduction
------------

This directory and its subdirectories contain code for the TinyPIM
application described in the book, __The C++ Standard Library from
Scratch__. The code is built incrementally through the book, with more
code introduced in each chapter. Each top-level directory contains the
code from a chapter in the book. These directories are broken into
subdirectories representing the steps of evolution within the
chapter. Look for "Code Notes" in the book for the directory names
containing code for a set of listings.

This code was tested using the Microsoft Visual C++ compiler, version
6.0 on Windows NT and the Egcs (Experimental GNU Compiler Suite) 1.1.2
compiler on Linux. See below for specific information for each
compiler. This code should also work with any conforming C++ compiler
and library.

If you discover any errors or have questions, please contact me at
stdlib-scratch@halpernwightsoftware.com. Don't forget to visit the
support page at
http://www.halpernwightsoftware.com/stdlib-scratch/support.html.

About the directory names
-------------------------

The fact that the directory names contain spaces, as in "Chapter 2
Code\strcpy Version" could be a headache for users of Unix command
shells. Although Unix allows spaces in file names, the shell will
normally treat spaces as delimiters between arguments. (The Windows
command shell does the same thing, but most people use the explorer or
IDE to manage files, rather than the command shell.) To manage this,
it is necessary to enclose any name containing spaces in quotes. For
example:

	$ cd Chapter 2 Code/strcpy Version

won't work, but

	$ cd "Chapter 2 Code/strcpy Version"

will work. (Note that Unix uses a foward slash (/) instead of a
backslash (\).)

The directory names are not particularly important, so feel free to
rename them if that will make your life easier. If there is ever a
second edition of this book, I will change the directory names to
avoid spaces.

Notes for users of the Microsoft Visual C++ compiler
----------------------------------------------------

The library that comes with the MSVC compiler (version 6.0) has a
number of bugs in it. In order to correctly run this code, you will
need the fixes available at: http://www.dinkumware.com/vc_fixes.html.

Even with the fixes, there are bugs in the library that have
work-arounds in this code. Most notably, the MSVC library does not put
names inherited from C into the std namespace. Page 38 of the book
describes how we worked around this deficiency.

The files in this directory include files ending in ".dsw", which are
MS Visual Studio workspace files and other files ending in ".dsp",
which are MS Visual Studio project files. In the Windows explorer,
double-clicing a .dsp file will bring up Visual Studio and will show
you all of the projects within a chapter. Right-click on one project
name and select "Make active" before attempting to compile
anything. Double-clicking on a .dsp file will bring up only one
project in Visual Studio.

Notes for users of the Egcs or GNU (GCC) compilers
--------------------------------------------------

Note: While the book was being written, the Egcs and GCC compilers
merged into a single compiler called GCC 2.95. This code has not been
tested on GCC 2.95. It should work, although some of the modifications
described below may be unnecessary. Both the knew compiler and the
older Egcs 1.1.2 compiler can be obtained from http://egcs.cygnus.com.

There are a few bugs and omissions in the library that comes with the
egcs compiler. This code set contains a directory, egcs-fixes, which
provide sufficient work-arounds and fixes for these problems to
compile the code. The files are as follows:

	sstream.h	An definition of stringstreams, which
			is missing from the library.
	sstream		A file that includes sstream.h
	sstream.cc	Implementation of stringstream. This is
			automatically included in sstream.h if the
			SSTREAM_INLINE preprocessor flag is set.
	auto_ptr.h	An implementation of auto_ptr, which
			is missing from the library.
	iomanipfixes.h	A implementation of some I/O manipulators
			which are missing from the library.
	rel_ops.h	An implementation of the rel_ops namespace,
			which is missing from the library.
	stringiofixes.h	A work-around for a bug in writing strings to
			output streams.

	libfixes.h	Includes all of the above except sstream.*.

To compile code without explicitly including the above files, use the
following compiler g++ command line flags to the:

  -I../../egcs-fixes -DSSTREAM_INLINE --include ../../egcs-fixes/libfixes.h

These flags are automatically supplied by the Makefiles contained
here. The same flags should be used to compile the exercises.
