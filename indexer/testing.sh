# Testing file for indexer and indextest
rm -f testfile0 testfile1
touch testfile0 testfile1

# No args
./indexer 

# One arg
./indexer onearg

# Two args
./indexer onearg twoarg threearg

# Incorrect Dir
./indexer wrongdir testfile

# Actual test
echo "Testing letters depth 0..."
./indexer ../crawler/test/letters/letters-0 testfile0

echo "Testing letters depth 1..."
./indexer ../crawler/test/letters/letters-1 testfile1
