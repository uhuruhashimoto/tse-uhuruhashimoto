# Testing file for common Crawler edge cases

seedA=http://cs50tse.cs.dartmouth.edu/tse/letters/
externalseed=http://google.com/
dirA=test
depthA=0
depthB=1
depthC=2
depthD=3
negdepth=-3
largedepth=100

# incorrect num of input arguments
# too few
./crawler 
./crawler $seedA 
./crawler $seedA $dirA
# too many
./crawler $seedA $dirA $depthA $depthA

# negative depth provided
./crawler $seedA $dirA $negdepth

# depth exceeds max (50)
./crawler $seedA $dirA $largedepth

# non-internal URL
./crawler $externalseed $dirA $depthA

################### TEST SITE ####################
# please note: if not in safe mode (file overwrite check enabled), 
# data will be overwritten and test directory will contain data from depth 3.

# DEPTH 0
./crawler $seedA $dirA $depthA
# DEPTH 1
./crawler $seedA $dirA $depthB
# DEPTH 2
./crawler $seedA $dirA $depthC
# DEPTH 3
./crawler $seedA $dirA $depthD


