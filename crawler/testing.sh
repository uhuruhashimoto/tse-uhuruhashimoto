# Testing file for common Crawler edge cases

seedA=http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
externalseed=http://google.com/
dir=test
dirA=$dir/letters
depthA=0
negdepth=-3
largedepth=100

####### CLEAN / CREATE DIRECTORIES #########
rm -rf $dirA-{0..3}

mkdir -p $dirA-{0..3}

# incorrect num of input arguments
# too few
./crawler 
./crawler $seedA 
./crawler $seedA $dirA
# too many
./crawler $seedA $dirA $depthA $depthA

# negative depth provided
./crawler $seedA $dirA-0 $negdepth

# depth exceeds max (50)
./crawler $seedA $dirA-0 $largedepth

# non-internal URL
./crawler $externalseed $dirA $depthA

################### TEST SITE ####################
# please note: if not in safe mode (file overwrite check enabled), 
# data may be overwritten.

# DEPTH 0
./crawler $seedA $dirA-0 0
# DEPTH 0 - SAME DIRECTORY (overwrite check)
./crawler $seedA $dirA-0 0
# DEPTH 1
./crawler $seedA $dirA-1 1
# DEPTH 2
./crawler $seedA $dirA-2 2
# DEPTH 3
./crawler $seedA $dirA-3 3


