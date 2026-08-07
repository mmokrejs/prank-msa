#ifndef MSP_PATCHLEVEL_H
#define MSP_PATCHLEVEL_H
// Local build marker for the mutation_scatter_plot pipeline.
// NOT upstream. Kept on the local/integration branch only, never on the
// branches that back the pull requests.
#define MSP_PATCHLEVEL \
"  local build: mutation_scatter_plot patch set 2026-08-07\n" \
"    +cxxflags-O3     fixes CXXFLAG typo that dropped -O3 (~4.3x faster)\n" \
"                     PR ariloytynoja/prank-msa#31\n" \
"    +version-check   -version no longer reports a 404 page as \"Found updates\"\n" \
"                     PR ariloytynoja/prank-msa#32\n" \
"    +probe-stdin     helper-binary probes no longer inherit stdin; without\n" \
"                     this, FastTree blocks forever on any non-EOF stdin\n" \
"                     not submitted upstream yet\n" \
"  NOT an upstream release; see docs/issues/prank_quirks.md\n"
#endif
