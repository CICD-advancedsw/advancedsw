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

#### 명령행 인수로 포트와 다른 DVM 설정 지정
```bash
# 포트 9000으로 실행 (다른 DVM 없이)
./build/app_program 9000

# 포트 9000으로 실행하고 다른 DVM 하나와 연결
./build/app_program 9000 172.20.10.2:9001

# 포트 9000으로 실행하고 여러 DVM과 연결
./build/app_program 9000 172.20.10.2:9001 192.168.1.100:9002

# 사용법: ./app_program <포트번호> [다른_DVM_IP:PORT] [다른_DVM_IP:PORT] ...
# 첫 번째 매개변수: 자신의 포트 번호
# 두 번째부터: 연결할 다른 DVM들의 IP:PORT
```

### 다중 DVM 실행 예시

여러 DVM을 동시에 실행하려면 각각 다른 포트를 사용해야 합니다.
이제 각 DVM의 포트를 매개변수로 지정할 수 있으므로, 같은 머신에서도 여러 DVM을 실행할 수 있습니다.

같은 머신에서 실행하는 예시:
```bash
# 터미널 1: 첫 번째 DVM (포트 9000)
./build/app_program 9000 127.0.0.1:9001

# 터미널 2: 두 번째 DVM (포트 9001) 
./build/app_program 9001 127.0.0.1:9000
```

다른 머신에서 실행하는 예시:
```bash
# 머신 1 (IP: 172.20.10.2): 첫 번째 DVM (포트 9000)
./build/app_program 9000 192.168.1.100:9001

# 머신 2 (IP: 192.168.1.100): 두 번째 DVM (포트 9001)
./build/app_program 9001 172.20.10.2:9000
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