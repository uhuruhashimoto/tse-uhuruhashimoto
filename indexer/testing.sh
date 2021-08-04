# Testing file for indexer and indextest

# No args
./indexer 

# One arg
./indexer onearg

# Two args
./indexer onearg twoarg threearg

# Incorrect Dir
./indexer wrongdir testfile

# Actual test
echo "Real Test..."
./indexer ../crawler/test/letters/letters-0 testfile

