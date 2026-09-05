#ifndef MSP_PATCHLEVEL_H
#define MSP_PATCHLEVEL_H
// Local build marker for the mutation_scatter_plot pipeline.
// NOT upstream. Kept on the local/integration branch only, never on the
// branches that back the pull requests.
#define MSP_PATCHLEVEL \
"  local build: mutation_scatter_plot patch set 2026-08-14\n" \
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
"  reproducibility: pass -reproducible (or any -seed=N, N>0). Without one,\n" \
"    prank seeds from time(0) and equal-scoring DP ties are broken at random:\n" \
"    measured 7 distinct alignments in 14 runs of one 3.8 kb pair.\n" \
"  NOT an upstream release; see docs/issues/prank_quirks.md\n"
#endif
