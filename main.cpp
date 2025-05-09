#include <iostream>
#include <list>
#include "src/app/presentation/controller.h"
#include "src/app/application/dvm.h"
#include "src/app/application/otherdvm.h"
#include "src/app/domain/location.h"

int main(void) {
    // // 위치 객체 생성
    // Location loc1(10, 20);
    // Location loc2(30, 40);
    // Location loc3(50, 60);
    
    // // OtherDVM 객체들 생성
    // OtherDVM otherDvm1(1, loc2);
    // OtherDVM otherDvm2(2, loc3);
    
    // // OtherDVM 리스트 생성
    // std::list<OtherDVM> otherDvms;
    // otherDvms.push_back(otherDvm1);
    // otherDvms.push_back(otherDvm2);
    
    // // DVM 객체 생성 (OtherDVM 의존성 주입)
    // DVM dvm(0, loc1, otherDvms);
    
    // // Controller 객체 생성 (DVM 의존성 주입)
    // Controller controller(&dvm);
    
    // 애플리케이션 실행
    // controller.run();
    
    return 0;
}