# Testing file for indexer and indextest
rm -f testfile0 testfile1 testfile2 newfile0 newfile1 newfile2
touch testfile0 testfile1 testfile2 newfile0 newfile1 newfile2

# Indexer

# No args
./indexer 

# One arg
./indexer onearg

# Three args
./indexer onearg twoarg threearg

# Incorrect Dir
./indexer wrongdir testfile

# Actual test
echo "Testing letters depth 0..."
./indexer ../crawler/test/letters/letters-0 testfile0

echo "Testing letters depth 1..."
./indexer ../crawler/test/letters/letters-1 testfile1

echo "Testing letters depth 2..."
./indexer ../crawler/test/letters/letters-2 testfile2

# Indextest

# No args
./indextest 

# One arg
./indextest onearg

# Three args
./indextest onearg twoarg threearg

echo "Testing indextest with depth 0..."
./indextest testfile0 newfile0

echo "Testing indextest with depth 1..."
./indextest testfile1 newfile1

echo "Testing indextest with depth 2..."
./indextest testfile2 newfile2

# Comparison

# sort and diff

