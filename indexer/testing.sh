# Testing file for indexer and indextest
rm -f testfile0 testfile1 newfile
touch testfile0 testfile1 newfile

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

# Indextest

# No args
./indextest 

# One arg
./indextest onearg

# Three args
./indextest onearg twoarg threearg

echo "Testing indextest with depth 0..."
./indextest testfile0 newfile

