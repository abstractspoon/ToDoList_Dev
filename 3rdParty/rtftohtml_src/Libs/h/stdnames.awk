BEGIN {
	count = 0
	printf "\n/* The following defines are automatically generated.  Do not edit. */\n\n"
	printf "\n/* These must be sequential beginning from zero */\n\n"
}
#/^$/ { continue }
#/^#/ { continue }
/^[^$#]/ {
	printf "#define rtfSC_%s\t%d\n", $1, count++
}
END { printf "\n#define rtfSC_MaxChar\t%d\n", count }
