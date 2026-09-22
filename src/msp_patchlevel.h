#ifndef MSP_PATCHLEVEL_H
#define MSP_PATCHLEVEL_H
// Local build marker for the mutation_scatter_plot pipeline.
// NOT upstream. Kept on the local/integration branch only, never on the
// branches that back the pull requests.
#define MSP_PATCHLEVEL \
"  local build: mutation_scatter_plot patch set 2026-09-21\n" \
"    +cxxflags-O3     fixes CXXFLAG typo that dropped -O3 (~4.3x faster)\n" \
"                     PR ariloytynoja/prank-msa#31\n" \
"    +version-check   -version no longer reports a 404 page as \"Found updates\"\n" \
"                     PR ariloytynoja/prank-msa#32\n" \
"    +fasttree        three fixes that each alone made FastTree unusable:\n" \
"                     helper-binary probes no longer inherit stdin (FastTree\n" \
"                     blocked forever on any non-EOF stdin); detection no\n" \
"                     longer requires the host to be named wasabi2; and\n" \
"                     -fasttree=NAME names the executable, since upstream\n" \
"                     ships FastTree/FastTreeMP/FastTreeUPGMA and none of\n" \
"                     them is called \"fasttree\"\n" \
"                     PR ariloytynoja/prank-msa#37\n" \
"    +keep-stop       a TERMINAL stop codon is masked NNN, not dropped; the\n" \
"                     61-state codon alphabet made it vanish, shortening the\n" \
"                     sequence by 3 with no column left to repair\n" \
"                     PR ariloytynoja/prank-msa#38\n" \
"    +rndbool-double  PwHirschberg::rndBool() used INTEGER division, so it\n" \
"                     returned false for every draw but one and its tie-break\n" \
"                     was inert; the two sibling copies already used double\n" \
"                     PR ariloytynoja/prank-msa#33\n" \
"    +reproducible    -reproducible now implies a fixed seed, so it engages the\n" \
"                     per-node hash seeding it was always meant to; before, it\n" \
"                     reseeded from a clock-derived value and was not\n" \
"                     reproducible, and it was absent from -help entirely\n" \
"                     PR ariloytynoja/prank-msa#34\n" \
"    +mafft-probe     \"is mafft available\" ran `mafft -h` under system(3),\n" \
"                     and mafft is a SHELL SCRIPT that unpacks itself to\n" \
"                     print help nobody reads; the probe is issued twice per\n" \
"                     process. Replaced by a PATH walk: -80 execve, -78\n" \
"                     clone, -29.7% wall per pair, output byte-identical\n" \
"                     on 30/30 pairs\n" \
"                     PR ariloytynoja/prank-msa#40\n" \
"    +hirschberg-stop an impossible Hirschberg state was PRINTED and then\n" \
"                     fallen through, seeding no matrix, so getMidSite() read\n" \
"                     values never written and maxCell.at(0) threw; now\n" \
"                     exit(-1) with the site and both sequence offsets\n" \
"                     PR ariloytynoja/prank-msa#35\n" \
"    +O3-not-dropped  a CXXFLAGS= on the make command line REPLACED the\n" \
"                     Makefile's flags (make gives the command line\n" \
"                     precedence, `+=` included), so every build made that\n" \
"                     way was -O2 despite +cxxflags-O3 above; `override`\n" \
"                     keeps -O3 and OPTFLAGS is the knob for -O0. Measured\n" \
"                     worth 4-6% on compute-bound runs\n" \
"    +mafft-failure   a failed initial alignment discarded mafft's stderr and\n" \
"                     then ran the WHOLE alignment again with 2>&1 to find\n" \
"                     out why, reporting a SECOND run's error on stdout, and\n" \
"                     then exit(0) -- so a prank that aligned nothing\n" \
"                     reported success. The failing run's own stderr is kept\n" \
"                     and reported on stderr; exit is 1\n" \
"                     PR ariloytynoja/prank-msa#TBD\n" \
"  reproducibility: pass -reproducible (or any -seed=N, N>0). Without one,\n" \
"    prank seeds from time(0) and equal-scoring DP ties are broken at random:\n" \
"    measured 7 distinct alignments in 14 runs of one 3.8 kb pair.\n" \
"  NOT an upstream release; see docs/issues/prank_quirks.md\n"
#endif
