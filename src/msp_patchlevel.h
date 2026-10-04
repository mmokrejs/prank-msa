#ifndef MSP_PATCHLEVEL_H
#define MSP_PATCHLEVEL_H
// Local build marker for the mutation_scatter_plot pipeline.
// NOT upstream. Kept on the local/integration branch only, never on the
// branches that back the pull requests. Every +entry with a PR number is one
// such branch, merged here; +O3-not-dropped is local build policy only.
#define MSP_PATCHLEVEL \
"  local build: mutation_scatter_plot patch set 2026-10-04\n" \
"    +cxxflags-O3     fixes CXXFLAG typo that dropped -O3 (~4.3x faster)\n" \
"                     PR ariloytynoja/prank-msa#31\n" \
"    +incpath         no -I/usr/include, which forced the system libc\n" \
"                     headers into a build with a non-system compiler\n" \
"                     PR ariloytynoja/prank-msa#39\n" \
"    +version-check   -version no longer reports a 404 page as \"Found\n" \
"                     updates\"; the status line is read whole and its code\n" \
"                     parsed; the banner names the host actually asked\n" \
"                     PR ariloytynoja/prank-msa#32\n" \
"    +fasttree        three fixes that each alone made FastTree unusable:\n" \
"                     helper-binary probes no longer inherit stdin (FastTree\n" \
"                     blocked forever on any non-EOF stdin); detection no\n" \
"                     longer requires the host to be named wasabi2; and\n" \
"                     -fasttree=NAME names the executable (passed to the\n" \
"                     shell as one word; it does not undo -nofasttree)\n" \
"                     PR ariloytynoja/prank-msa#37\n" \
"    +keep-stop       an unknown TERMINAL codon (a stop) is kept as a column\n" \
"                     written NNN, not dropped; the note says so, -keep too\n" \
"                     PR ariloytynoja/prank-msa#38\n" \
"    +rndbool-double  PwHirschberg::rndBool() used INTEGER division, so it\n" \
"                     returned false for every draw but one and its tie-break\n" \
"                     was inert; the two sibling copies already used double\n" \
"                     PR ariloytynoja/prank-msa#33\n" \
"    +reproducible    -reproducible implies a fixed seed and so the per-node\n" \
"                     hash seeding: every tie broken the same way each run\n" \
"                     (for a given guide tree); -seed=# must be positive\n" \
"                     PR ariloytynoja/prank-msa#34\n" \
"    +hirschberg-stop an impossible Hirschberg state, or an ancestral site\n" \
"                     with no choosable character, was printed and fallen\n" \
"                     through (uninitialised matrices; a string function\n" \
"                     returning nothing); both now stop with exit(-1)\n" \
"                     PR ariloytynoja/prank-msa#35\n" \
"    +mafft-probe     \"is mafft available\" ran the mafft SHELL SCRIPT twice\n" \
"                     per process; now a PATH walk for a regular executable\n" \
"                     file: -80 execve, -29.7% wall per pair, output\n" \
"                     byte-identical\n" \
"                     PR ariloytynoja/prank-msa#40\n" \
"    +tool-probes     the exonerate and raxml probes use the same walk instead\n" \
"                     of running those binaries: -11 execve per pair\n" \
"                     PR ariloytynoja/prank-msa#42\n" \
"    +mafft-failure   a failed initial alignment re-ran the whole alignment\n" \
"                     for its error message and then exit(0); the failing\n" \
"                     run's own stderr is reported, mafft's exit status is\n" \
"                     checked, the run's temp dir removed, and exit is 1\n" \
"                     PR ariloytynoja/prank-msa#41\n" \
"    +O3-not-dropped  a CXXFLAGS= on the make command line REPLACED the\n" \
"                     Makefile's flags (make gives the command line\n" \
"                     precedence, `+=` included), so every build made that\n" \
"                     way was -O2 despite +cxxflags-O3 above; `override`\n" \
"                     keeps -O3 and OPTFLAGS is the knob for -O0. Measured\n" \
"                     worth 4-6% on compute-bound runs. Local only.\n" \
"  reproducibility: pass -reproducible (or any -seed=N, N>0). Without one,\n" \
"    prank seeds from time(0) and equal-scoring DP ties are broken at random:\n" \
"    measured 7 distinct alignments in 14 runs of one 3.8 kb pair.\n" \
"  NOT an upstream release; see docs/issues/prank_quirks.md\n"
#endif
