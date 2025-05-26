# advancedsw

분산 자판기 시스템 (Distributed Vending Machine System)

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

#### 명령행 인수로 다른 DVM 설정 지정
```bash
# 기본 설정으로 실행 (다른 DVM 없이)
./build/app_program

# 다른 DVM 하나와 연결
./build/app_program 172.20.10.2:9001

# 여러 DVM과 연결
./build/app_program 172.20.10.2:9001 192.168.1.100:9002

# 사용법: ./app_program [다른_DVM_IP:PORT] [다른_DVM_IP:PORT] ...
# 주의: 자신의 포트는 9000으로 고정되어 있습니다.
```

### 다중 DVM 실행 예시

여러 DVM을 동시에 실행하려면 각각 다른 포트를 사용해야 합니다.
**현재 버전에서는 자신의 포트가 9000으로 하드코딩되어 있으므로, 여러 DVM을 같은 머신에서 실행할 수 없습니다.**

다른 머신에서 실행하는 예시:
```bash
# 머신 1 (IP: 172.20.10.2): 첫 번째 DVM
./build/app_program 192.168.1.100:9000

# 머신 2 (IP: 192.168.1.100): 두 번째 DVM  
./build/app_program 172.20.10.2:9000
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