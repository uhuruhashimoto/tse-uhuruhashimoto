# Testing

# wrong number of args
./querier

./querier arg1 arg2 arg3

# incorrect directory
./querier falsedirectory newfile

# incorrect file
./querier ../crawler/test/letters/letters-0 wrongfile

# Test - letters depth 0
echo "Testing with letters depth 0..."
./querier ../crawler/test/letters/letters-0 ../indexer/test/letters/index.letters-0