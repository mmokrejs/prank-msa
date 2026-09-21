#############################################################################
# Makefile for building: prank
#############################################################################

####### Compiler, tools and options

CC           ?= gcc
CXX          ?= g++
DEFINES       = -DNDEBUG
CPPFLAGS     += $(DEFINES)
CFLAGS       += -pipe -O3

# Optimisation, chosen by measurement rather than by taste. Change it through
# OPTFLAGS, NOT through CXXFLAGS -- see the "how to override" note below.
#
# Measured 2026-09-21, gcc 14.2, all builds from identical sources. Every
# variant below produced BYTE-IDENTICAL alignments on every input tried, so
# this is purely a speed question and nothing here can change a result.
#
# READ THE NOISE FLOOR BEFORE READING THE NUMBERS. `-ftree-vectorize` is a
# literal no-op at -O3 (proven below), so the -O3 and -O3-ftree-vectorize
# binaries are byte-identical and ANY difference measured between them is
# pure measurement noise. That is the control, and it is what makes the rest
# interpretable:
#
#   wall time, minimum of N runs, vs -O2:
#                          pair (-codon)   30 seqs (-codon)   genome (-DNA)
#     time at -O2               0.157 s        21.52 s          18.82 s
#     -O3                       -0.3%          -6.2%            -4.4%
#     -O3 -ftree-vectorize      +2.7%          -6.5%            -4.3%
#     -O3 -flto                 +2.4%          -5.1%            -5.8%
#     noise floor (control)      3.0 pp         0.3 pp           0.1 pp
#
#   So the pair case measures NOTHING -- at 0.157 s its noise floor swamps
#   every effect. Only the two long runs discriminate, and there -O3 is worth
#   a real 4-6%. That is why it is kept.
#
#   -O3 -flto is NOT kept, and the reason is a trap worth stating. By
#   INSTRUCTION COUNT (cachegrind, deterministic) it looks like the clear
#   winner -- -17.6% on the pair case against -3.5% for plain -O3. It is also
#   2.4% SLOWER there in wall time. Instruction count is not a proxy for time:
#   LTO's cross-unit inlining removes instructions and pays for them in
#   I-cache pressure. Nor is the pair case startup-bound, which was the first
#   explanation offered and was wrong -- getrusage puts 83% of its wall in
#   USER time. On the two long runs LTO is genuinely mixed: 1.1 pp worse than
#   plain -O3 on codon, 1.4 pp better on DNA, both outside the noise floor.
#   A flag that is better at one model and worse at the other, needs matching
#   LFLAGS at link time, and costs 0.5 s of an 8 s build, is not worth it.
#
#   -march=x86-64-v3 is NOT kept, and NOT because the vector registers go
#   unused -- objdump finds 525 ymm instructions, 95 of them inside
#   Hirschberg::getMidSite, the hot DP function. They simply do not pay: 0.02
#   percentage points of instruction count over plain -O3 on a 12-sequence
#   MSA, i.e. 35 M instructions out of 136 G, because that loop is memory-
#   and branch-bound rather than arithmetic-bound. Against a SIGILL on any
#   execution node without AVX2, 0.02% is not a trade worth making.
#
#   -ftree-vectorize is NOT added, because at -O3 it does nothing at all:
#   `-O3` and `-O3 -ftree-vectorize` give byte-identical disassembly and the
#   same .text size (0xdf5d5). It is NOT redundant at -O2, where it lifts the
#   vectoriser cost model from very-cheap to cheap -- but this build is -O3.
#   (Do not trust `gcc -Q --help=optimizers` here: it reports the flag as
#   [disabled] at both levels, which is why this was checked against the
#   emitted code instead.)
#
#   -Ofast is NOT kept, and this is the one to leave alone regardless of its
#   numbers (which are: no gain over -O3). It implies -ffast-math, permitting
#   floating-point reassociation; PRANK's Hirschberg DP picks a path by
#   comparing accumulated log-probabilities, so a moved last bit can flip a
#   tie and shift a gap.
#
# -ggdb is deliberate and costs no run time: it only adds debug info to the
# binary. It is what makes a core file from a cluster run readable, and those
# cores are the only evidence available when a batch dies on an execution node.
#
# NOT included on purpose: -w. It silences every warning, and this build emits
# 26 "no return statement in function returning non-void". They are all the
# SAME function, re-reported once per translation unit that includes the
# header: treenode.h:365, `virtual bool updateInsertionSite(int, bool) {}` --
# a bool returning nothing, which is undefined behaviour if reached. It is not
# reachable (TreeNode has exactly two subclasses, both override it, and
# TreeNode is never instantiated), which is the specific ground on which
# raising -O2 to -O3 is safe here rather than merely "the output did not
# change". Add -w to a command line if you want a quiet build; do not bake it
# in here, or the next such warning is invisible too.
OPTFLAGS     ?= -std=c++11 -pipe -O3 -ggdb

# HOW TO OVERRIDE, and why this is `override` rather than a plain `+=`.
#
# make gives a variable set on the COMMAND LINE precedence over any assignment
# in the makefile, `+=` included. So the obvious-looking
#
#     make CXXFLAGS="-std=c++11 -pipe -w -O2 -ggdb"
#
# does not ADD to the line above, it REPLACES it -- and every such build was
# silently an -O2 build, discarding the -O3 this file has asked for since the
# CXXFLAG/CXXFLAGS typo was fixed. That is how the optimisation was lost once
# already, with nothing in the output to show it.
#
# `override` makes the flags above survive such a command line. They are
# appended last, and gcc honours the LAST -O it is given, so an accidental
# -O2 on the command line no longer wins. To build unoptimised on purpose,
# set the knob that is meant for it:
#
#     make OPTFLAGS="-std=c++11 -pipe -O0 -ggdb"
override CXXFLAGS += $(OPTFLAGS)
# -I. only. An explicit -I/usr/include is redundant with any compiler's own
# search path, and actively wrong when the compiler is not the system one:
# it forces the SYSTEM libc headers into a build whose libstdc++ came from
# elsewhere. prank links no external library, so it needs no -I but its own.
INCPATH       = -I.
LINK         ?= $(CXX)
#LFLAGS       = -m64
LIBS          = $(SUBLIBS)    
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
POD2MAN       = pod2man --utf8 --center "The Probabilistic Alignment Kit" -release="v.121211"

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = writefile.cpp \
		treenode.cpp \
		translatesequences.cpp \
		terminalsequence.cpp \
		terminalnode.cpp \
		site.cpp \
		sequence.cpp \
		readnewick.cpp \
		readfile.cpp \
		readalignment.cpp \
		pwsite.cpp \
		pwhirschberg.cpp \
		progressivealignment.cpp \
		prank.cpp \
		postprobability.cpp \
		phylomatchscore.cpp \
		node.cpp \
		intmatrix.cpp \
		hmmodel.cpp \
		hirschberg.cpp \
		guidetree.cpp \
		fullprobability.cpp \
		flmatrix.cpp \
		eigen.cpp \
		dbmatrix.cpp \
		characterprobability.cpp \
		boolmatrix.cpp \
		ancestralsequence.cpp \
		ancestralnode.cpp \
		check_version.cpp \
		exonerate_reads.cpp \
		mafft_alignment.cpp \
		bppancestors.cpp \
		raxmlancestors.cpp \
		fasttree_tree.cpp \
		raxmlrebl.cpp 
OBJECTS       = writefile.o \
		treenode.o \
		translatesequences.o \
		terminalsequence.o \
		terminalnode.o \
		site.o \
		sequence.o \
		readnewick.o \
		readfile.o \
		readalignment.o \
		pwsite.o \
		pwhirschberg.o \
		progressivealignment.o \
		prank.o \
		postprobability.o \
		phylomatchscore.o \
		node.o \
		intmatrix.o \
		hmmodel.o \
		hirschberg.o \
		guidetree.o \
		fullprobability.o \
		flmatrix.o \
		eigen.o \
		dbmatrix.o \
		characterprobability.o \
		boolmatrix.o \
		ancestralsequence.o \
		ancestralnode.o \
		check_version.o \
		exonerate_reads.o \
		mafft_alignment.o \
		bppancestors.o \
		raxmlancestors.o \
		fasttree_tree.o \
		raxmlrebl.o
DESTDIR       = 
TARGET        = prank

MANPAGES      = prank.1

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules


all: Makefile $(TARGET) $(MANPAGES)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS) $(LDFLAGS)


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

compiler_clean: 

####### Compile

writefile.o: writefile.cpp writefile.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o writefile.o writefile.cpp

treenode.o: treenode.cpp config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		writefile.h \
		treenode.h \
		sequence.h \
		site.h \
		boolmatrix.h \
		hirschberg.h \
		phylomatchscore.h \
		ancestralsequence.h \
		terminalsequence.h \
		fullprobability.h \
		postprobability.h \
		characterprobability.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o treenode.o treenode.cpp

translatesequences.o: translatesequences.cpp translatesequences.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o translatesequences.o translatesequences.cpp

terminalsequence.o: terminalsequence.cpp terminalsequence.h \
		sequence.h \
		site.h \
		intmatrix.h \
		flmatrix.h \
		dbmatrix.h \
		boolmatrix.h \
		config.h \
		hmmodel.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o terminalsequence.o terminalsequence.cpp

terminalnode.o: terminalnode.cpp terminalnode.h \
		treenode.h \
		sequence.h \
		site.h \
		intmatrix.h \
		flmatrix.h \
		dbmatrix.h \
		boolmatrix.h \
		terminalsequence.h \
		config.h \
		hmmodel.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o terminalnode.o terminalnode.cpp

site.o: site.cpp site.h \
		intmatrix.h \
		flmatrix.h \
		dbmatrix.h \
		boolmatrix.h \
		hmmodel.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o site.o site.cpp

sequence.o: sequence.cpp sequence.h \
		site.h \
		intmatrix.h \
		flmatrix.h \
		dbmatrix.h \
		boolmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o sequence.o sequence.cpp

readnewick.o: readnewick.cpp readnewick.h \
		treenode.h \
		sequence.h \
		site.h \
		intmatrix.h \
		flmatrix.h \
		dbmatrix.h \
		boolmatrix.h \
		ancestralnode.h \
		node.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o readnewick.o readnewick.cpp

readfile.o: readfile.cpp readfile.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o readfile.o readfile.cpp

readalignment.o: readalignment.cpp readalignment.h \
		sequence.h \
		site.h \
		intmatrix.h \
		flmatrix.h \
		dbmatrix.h \
		boolmatrix.h \
		treenode.h \
		phylomatchscore.h \
		ancestralsequence.h \
		terminalsequence.h \
		config.h \
		hmmodel.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o readalignment.o readalignment.cpp

pwsite.o: pwsite.cpp pwsite.h \
		flmatrix.h \
		intmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o pwsite.o pwsite.cpp

pwhirschberg.o: pwhirschberg.cpp config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		pwhirschberg.h \
		pwsite.h \
		exonerate_reads.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o pwhirschberg.o pwhirschberg.cpp

progressivealignment.o: progressivealignment.cpp readnewick.h \
		treenode.h \
		sequence.h \
		site.h \
		intmatrix.h \
		flmatrix.h \
		dbmatrix.h \
		boolmatrix.h \
		readfile.h \
		writefile.h \
		ancestralnode.h \
		guidetree.h \
		progressivealignment.h \
		config.h \
		hmmodel.h \
		translatesequences.h \
		node.h \
		mafft_alignment.h \
		exonerate_reads.h \
		bppancestors.h \
		raxmlancestors.h \
		fasttree_tree.h \
		readalignment.h \
		phylomatchscore.h \
		ancestralsequence.h \
		terminalsequence.h \
		hirschberg.h \
		raxmlrebl.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o progressivealignment.o progressivealignment.cpp

prank.o: prank.cpp progressivealignment.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		treenode.h \
		sequence.h \
		site.h \
		boolmatrix.h \
		readfile.h \
		writefile.h \
		readnewick.h \
		guidetree.h \
		translatesequences.h \
		node.h \
		mafft_alignment.h \
		exonerate_reads.h \
		bppancestors.h \
		raxmlancestors.h \
		fasttree_tree.h \
		readalignment.h \
		phylomatchscore.h \
		ancestralsequence.h \
		terminalsequence.h \
		hirschberg.h \
		check_version.h \
		prank.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o prank.o prank.cpp

postprobability.o: postprobability.cpp config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		postprobability.h \
		sequence.h \
		site.h \
		boolmatrix.h \
		phylomatchscore.h \
		ancestralsequence.h \
		terminalsequence.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o postprobability.o postprobability.cpp

phylomatchscore.o: phylomatchscore.cpp config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		phylomatchscore.h \
		sequence.h \
		site.h \
		boolmatrix.h \
		ancestralsequence.h \
		terminalsequence.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o phylomatchscore.o phylomatchscore.cpp

node.o: node.cpp node.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o node.o node.cpp

intmatrix.o: intmatrix.cpp intmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o intmatrix.o intmatrix.cpp

hmmodel.o: hmmodel.cpp hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		eigen.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o hmmodel.o hmmodel.cpp

hirschberg.o: hirschberg.cpp config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		exonerate_reads.h \
		hirschberg.h \
		sequence.h \
		site.h \
		boolmatrix.h \
		phylomatchscore.h \
		ancestralsequence.h \
		terminalsequence.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o hirschberg.o hirschberg.cpp

guidetree.o: guidetree.cpp guidetree.h \
		flmatrix.h \
		intmatrix.h \
		fasttree_tree.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		ancestralnode.h \
		pwhirschberg.h \
		pwsite.h \
		translatesequences.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o guidetree.o guidetree.cpp

fullprobability.o: fullprobability.cpp config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		fullprobability.h \
		sequence.h \
		site.h \
		boolmatrix.h \
		phylomatchscore.h \
		ancestralsequence.h \
		terminalsequence.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o fullprobability.o fullprobability.cpp

flmatrix.o: flmatrix.cpp flmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o flmatrix.o flmatrix.cpp

eigen.o: eigen.cpp eigen.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o eigen.o eigen.cpp

dbmatrix.o: dbmatrix.cpp dbmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o dbmatrix.o dbmatrix.cpp

characterprobability.o: characterprobability.cpp config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		characterprobability.h \
		sequence.h \
		site.h \
		boolmatrix.h \
		ancestralsequence.h \
		terminalsequence.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o characterprobability.o characterprobability.cpp

boolmatrix.o: boolmatrix.cpp boolmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o boolmatrix.o boolmatrix.cpp

ancestralsequence.o: ancestralsequence.cpp ancestralsequence.h \
		sequence.h \
		site.h \
		intmatrix.h \
		flmatrix.h \
		dbmatrix.h \
		boolmatrix.h \
		config.h \
		hmmodel.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o ancestralsequence.o ancestralsequence.cpp

ancestralnode.o: ancestralnode.cpp config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		writefile.h \
		treenode.h \
		sequence.h \
		site.h \
		boolmatrix.h \
		hirschberg.h \
		phylomatchscore.h \
		ancestralsequence.h \
		terminalsequence.h \
		fullprobability.h \
		postprobability.h \
		characterprobability.h \
		terminalnode.h \
		readalignment.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o ancestralnode.o ancestralnode.cpp

check_version.o: check_version.cpp check_version.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o check_version.o check_version.cpp

exonerate_reads.o: exonerate_reads.cpp exonerate_reads.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h \
		translatesequences.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o exonerate_reads.o exonerate_reads.cpp

mafft_alignment.o: mafft_alignment.cpp mafft_alignment.h \
		tool_probe.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o mafft_alignment.o mafft_alignment.cpp

bppancestors.o: bppancestors.cpp bppancestors.h \
		ancestralnode.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		readfile.h \
		readnewick.h \
		treenode.h \
		sequence.h \
		site.h \
		boolmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o bppancestors.o bppancestors.cpp

raxmlancestors.o: raxmlancestors.cpp raxmlancestors.h \
		ancestralnode.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		readfile.h \
		readnewick.h \
		treenode.h \
		sequence.h \
		site.h \
		boolmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o raxmlancestors.o raxmlancestors.cpp

fasttree_tree.o: fasttree_tree.cpp fasttree_tree.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		ancestralnode.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o fasttree_tree.o fasttree_tree.cpp

raxmlrebl.o: raxmlrebl.cpp raxmlrebl.h \
		ancestralnode.h \
		config.h \
		hmmodel.h \
		dbmatrix.h \
		flmatrix.h \
		intmatrix.h \
		node.h \
		readfile.h \
		readnewick.h \
		treenode.h \
		sequence.h \
		site.h \
		boolmatrix.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCPATH) -o raxmlrebl.o raxmlrebl.cpp

####### Manpages

%.1: %.1.pod
	$(POD2MAN) --section=1 --name="$(shell basename $@ .1 | tr a-z A-Z)" $< >$@

doc: $(MANPAGES)


####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

