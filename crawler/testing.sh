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

# safe mode - file overwrite check enabled
#make safe
#./crawler $seedA $dirA $depthA
#./crawler $seedA $dirA $depthA
#make clean

################### TEST SITE ####################
# DEPTH 0
./crawler $seedA $dirA $depthA
# DEPTH 1
./crawler $seedA $dirA $depthB
# DEPTH 2
./crawler $seedA $dirA $depthC
# DEPTH 3
./crawler $seedA $dirA $depthD


