# Testing file for common Crawler edge cases

### Uncomment code at line 67 to run wiki at depth 2. This test creates 1675 files 
# under test/wiki/wiki-2 and takes ~30 minutes to run (with the build-in pagefetcher delay). 
# If not active, directory wiki-2 will simply be created empty. ###

seedA=http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
seedB=http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html
seedC=http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html
externalseed=http://google.com/
dir=test
dirA=$dir/letters/letters
dirB=$dir/wiki/wiki
dirC=$dir/toscrape/toscrape
depthA=0
negdepth=-3
largedepth=100

####### CLEAN / CREATE DIRECTORIES #########
rm -rf $dirA-{0..3}
rm -rf $dirB{0..2}
rm -rf $dirC{0..1}

mkdir -p $dirA-{0..3}
mkdir -p $dirB-{0..2}
mkdir -p $dirC-{0..1}

# incorrect num of input arguments
# too few
./crawler 
./crawler $seedA 
./crawler $seedA $dirA-0

# too many
./crawler $seedA $dirA-0 $depthA $depthA

# negative depth provided
./crawler $seedA $dirA-0 $negdepth

# depth exceeds max (50)
./crawler $seedA $dirA-0 $largedepth

# non-internal URL
./crawler $externalseed $dirA-0 $depthA

################### TEST SITE ####################
# please note: if not in safe mode (file overwrite check enabled), 
# data may be overwritten.

##### LETTERS #####
# DEPTH 0
./crawler $seedA $dirA-0 0
# DEPTH 0 - SAME DIRECTORY (overwrite warning should be seen in testing.out)
./crawler $seedA $dirA-0 0
# DEPTH 1
./crawler $seedA $dirA-1 1
# DEPTH 2
./crawler $seedA $dirA-2 2
# DEPTH 3
./crawler $seedA $dirA-3 3

##### WIKI #####
# DEPTH 0
./crawler $seedB $dirB-0 0
# DEPTH 1
./crawler $seedB $dirB-1 1
# DEPTH 2
#./crawler $seedB $dirB-2 2

##### TOSCRAPE #####
# DEPTH 0
./crawler $seedC $dirC-0 0
# DEPTH 1
./crawler $seedC $dirC-1 1




