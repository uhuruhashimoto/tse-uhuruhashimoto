#!/bin/bash
# Testing file for indexer and indextest

############### Variables ##################

# General data path
dir="../crawler/test" 
letters="/letters/letters" # letters (-0)
toscrape="/toscrape/toscrape" # toscrape (-0)
wiki="/wiki/wiki" # wiki (-0)

# Indexes (indexer)
indexdir="./test"
indexletters="/letters/index.letters" # letters (-0)
indextoscrape="/toscrape/index.toscrape" # toscrape (-0)
indexwiki="/wiki/index.wiki" # wiki (-0)

# Copies (indextest)
copyletters="/letters/copyindex.letters" # letters (-0)
copytoscrape="/toscrape/copyindex.toscrape" # toscrape (-0)
copywiki="/wiki/copyindex.wiki" # wiki (-0)

# Prepare workspace: remove indexes and copies
rm -rf $indexdir
mkdir -p $indexdir/letters $indexdir/toscrape $indexdir/wiki
touch $indexdir$indexletters-{0..3} $indexdir$indextoscrape-{0..1} $indexdir$indexwiki-{0..1}
touch $indexdir$copyletters-{0..3} $indexdir$copytoscrape-{0..1} $indexdir$copywiki-{0..1}

############### Indexer ##################

# No args
./indexer 

# One arg
./indexer onearg

# Three args
./indexer onearg twoarg threearg

# Incorrect Dir
./indexer wrongdir testfile

# Actual tests
echo "Testing letters..."
./indexer $dir$letters-0 $indexdir$indexletters-0
./indexer $dir$letters-1 $indexdir$indexletters-1
./indexer $dir$letters-2 $indexdir$indexletters-2
./indexer $dir$letters-3 $indexdir$indexletters-3

echo "Testing toscrape..."
./indexer $dir$toscrape-0 $indexdir$indextoscrape-0
./indexer $dir$toscrape-0 $indexdir$indextoscrape-0

echo "Testing wiki..."
./indexer $dir$wiki-0 $indexdir$indexwiki-0
./indexer $dir$wiki-0 $indexdir$indexwiki-0

############### Indextest ##################

# No args
./indextest 

# One arg
./indextest onearg

# Three args
./indextest onearg twoarg threearg

echo "Testing letters..."
./indextest $indexdir$indexletters-0 $indexdir$copyletters-0
./indextest $indexdir$indexletters-1 $indexdir$copyletters-1
./indextest $indexdir$indexletters-2 $indexdir$copyletters-2
./indextest $indexdir$indexletters-3 $indexdir$copyletters-3

echo "Testing toscrape..."
./indextest $indexdir$indextoscrape-0 $indexdir$copytoscrape-0
./indextest $indexdir$indextoscrape-1 $indexdir$copytoscrape-1

echo "Testing wiki..."
./indextest $indexdir$indexwiki-0 $indexdir$copywiki-0
./indextest $indexdir$indexwiki-1 $indexdir$copywiki-2
############### Comparison ##################

echo "Comparing files..."
a=$(diff <(sort $indexdir$indexletters-0) <(sort $indexdir$copyletters-0))
b=$(diff <(sort $indexdir$indexletters-1) <(sort $indexdir$copyletters-1))
c=$(diff <(sort $indexdir$indexletters-2) <(sort $indexdir$copyletters-2))
d=$(diff <(sort $indexdir$indexletters-3) <(sort $indexdir$copyletters-3))

if [[ $a == "" && $b == "" && $c == "" && $d == "" ]]; then
    echo "letters test successful!"
fi

a=$(diff <(sort $indexdir$indextoscrape-0) <(sort $indexdir$copytoscrape-0))
b=$(diff <(sort $indexdir$indextoscrape-1) <(sort $indexdir$copytoscrape-1))


if [[ $a == "" && $b == "" ]]
then
    echo "toscrape test successful!"
fi

a=$(diff <(sort $indexdir$indexwiki-0) <(sort $indexdir$copywiki-0))
b=$(diff <(sort $indexdir$indexwiki-1) <(sort $indexdir$copywiki-1))

if [[ $a == "" && $b == "" ]]
then
    echo "wiki test successful!"
fi
