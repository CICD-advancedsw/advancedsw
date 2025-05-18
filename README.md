# advancedsw

test test github test kyunghee modified
test on 20250503
more test using push
clang-tidy test push
new clang tidy test kyunghee push
clang tidy code until install
cppcheck test
cppcheck second test with new yaml code
cppcheck third test 
cppcheck 4th test
cppcheck 5th test
sonarcloud first test
sonarcloud 2nd test
sonarcloud 3rd test
sonarcloud 4th test
sonarcloud 5th test
sonarcloud 6th test
soncarcloud 7th test
sonarcloud 8th test
clangtidy 2nd test
clangtidy 3rd test
clangtidy 4th test
cppcheck 6th test
slack test
clang tidy slack test
cppcheck slack test
sonarcloud local zip test
sonarcloud slack test
clang tidy send failure message to slack
final test
clang tidy slack message test

## Build, Run and Test Instructions

### One-command Build & Test
To build the project and run all tests at once, use the following commands:

```bash
mkdir -p build && cd build
cmake ..
make
./dvm_test && ./sale_test && ./domain_test && ./otherdvm_test && ./controller_test && ./integration_test
```

### Individual Components

1. Build only:
```bash
mkdir -p build && cd build
cmake ..
make
```

2. Run main application:
```bash
./app_program
```

3. Run individual test suites:
```bash
# Run DVM tests
./dvm_test

# Run Sale tests
./sale_test

# Run Domain tests
./domain_test

# Run OtherDVM tests
./otherdvm_test

# Run Controller tests
./controller_test

# Run Integration tests
./integration_test
```

All test executables support Google Test command line options. For more detailed test output, you can use:
```bash
./[test_name] --gtest_color=yes --gtest_print_time=1
```
```