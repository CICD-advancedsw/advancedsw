#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../app/application/dvm.h"
#include "../app/domain/item.h"
#include "../app/domain/location.h"
#include "../app/dto.h"
#include "../app/exception/dvmexception.h"
#include <list>
#include <map>
#include <string>
#include <stdexcept> // std::runtime_error

using namespace std;
using ::testing::_;
using ::testing::Return;

// 테스트용 Mock OtherDVM 클래스
class MockOtherDVM : public OtherDVM {
public:
    MockOtherDVM(int id, Location loc) : OtherDVM(id, loc) {}

    MOCK_METHOD(CheckStockResponse, findAvailableStocks, (const CheckStockRequest& request));
    MOCK_METHOD(askPrepaymentResponse, askForPrepayment, (const askPrepaymentRequest& request));
};


class DVMTest : public ::testing::Test {
protected:
    DVM* dvm1; // 주 테스트 대상 DVM
    DVM* dvm_for_other_interaction; // 다른 DVM 역할을 할 DVM (필요시)

    Item item_coke{"001", "Coke", 1000};
    Item item_sprite{"002", "Sprite", 1200};
    Item item_fanta{"003", "Fanta", 1100}; // 재고 없는 아이템
    Item item_pepsi{"004", "Pepsi", 1050}; // 다른 DVM에만 있는 아이템

    list<OtherDVM> other_dvm_list_empty;
    list<OtherDVM> other_dvm_list_with_one;
    
    // MockOtherDVM 포인터로 관리하여 수명 관리
    unique_ptr<MockOtherDVM> mock_other_dvm_inst;


    virtual void SetUp() {
        list<Item> items1 = {item_coke, item_sprite, item_fanta};
        map<Item, int> stocks1 = {
            {item_coke, 10},
            {item_sprite, 5},
            {item_fanta, 0} // Fanta는 재고 없음
        };
        
        // MockOtherDVM 인스턴스 생성 및 설정
        mock_other_dvm_inst = make_unique<MockOtherDVM>(2, Location(10, 10));

        dvm1 = new DVM(1, Location(0,0), other_dvm_list_empty, items1, stocks1, {});

        // 다른 DVM 역할을 할 DVM 인스턴스 (예: 선결제 요청 대상)
        list<Item> items_for_other = {item_pepsi};
        map<Item, int> stocks_for_other = {{item_pepsi, 20}};
        OtherDVM real_other_dvm(2, Location(10,10)); 
    }

    virtual void TearDown() {
        delete dvm1;
        dvm1 = nullptr;
    }
};

// TC-UI-001: Display all beverage menu
TEST_F(DVMTest, QueryItems_ShouldReturnAllItemInfo) {
    string result = dvm1->queryItems();
    EXPECT_NE(result.find(item_coke.printItem()), string::npos);
    EXPECT_NE(result.find(item_sprite.printItem()), string::npos);
    EXPECT_NE(result.find(item_fanta.printItem()), string::npos); // 재고가 없어도 메뉴에는 표시
}

// TC-UI-002 / TC-UI-003: Display beverage stock info (available)
TEST_F(DVMTest, QueryStocks_AvailableInCurrentDVM_ShouldReturnThisDVMInfo) {
    string result = dvm1->queryStocks(item_coke.getItemCode(), 1);
    EXPECT_NE(result.find("flag:this"), string::npos);
    EXPECT_NE(result.find("item_code:" + item_coke.getItemCode()), string::npos);
    EXPECT_NE(result.find("total_price:" + to_string(item_coke.getPrice())), string::npos); // 1개 가격
    EXPECT_NE(result.find("item_name:" + item_coke.printItem()), string::npos);
    EXPECT_NE(result.find("count:1"), string::npos);
}

// TC-UI-004 / TC-DIST-001 / TC-DIST-004: Query stocks for out-of-stock item, no other DVMs
TEST_F(DVMTest, QueryStocks_OutOfStockInCurrentDVM_NoOtherDVMs_ShouldReturnNotAvailable) {
    string result = dvm1->queryStocks(item_fanta.getItemCode(), 1); // Fanta는 재고 0
    EXPECT_NE(result.find("flag:not_available"), string::npos);
    EXPECT_NE(result.find("item_code:" + item_fanta.getItemCode()), string::npos);
}


// TC-SALES-001: Direct sales processing (sufficient stock)
TEST_F(DVMTest, RequestOrder_SufficientStock_ShouldProcessSale) {
    SaleRequest request{item_coke.getItemCode(), 2, item_coke}; // item_coke는 생성자에서 DVM에 설정된 item1과 동일해야 함
    
    EXPECT_NO_THROW(dvm1->requestOrder(request));
    
    SaleRequest request_fail{item_coke.getItemCode(), 9, item_coke}; 
    EXPECT_THROW(dvm1->requestOrder(request_fail), runtime_error); // Insufficient stock
}

// TC-SALES-001: 재고 충분할 때 구매 성공 테스트
TEST_F(DVMTest, RequestOrder_InsufficientStock_ShouldThrowError) {
    SaleRequest request{item_sprite.getItemCode(), 10, item_sprite}; // Sprite 재고 5개
    EXPECT_THROW(dvm1->requestOrder(request), runtime_error); // Insufficient stock
}

// TC-PRE-006: Auth code verification (successful)
TEST_F(DVMTest, ProcessPrepaidItem_ValidCertCode_ShouldReturnTrue) {
    string cert_code_for_test = "TESTCODE123";
    dvm1->saveSaleFromOther(item_coke.getItemCode(), 1, cert_code_for_test); // DVM1에 판매 기록
    EXPECT_TRUE(dvm1->processPrepaidItem(cert_code_for_test));
}

// TC-PRE-007: Invalid auth code handling
TEST_F(DVMTest, ProcessPrepaidItem_InvalidCertCode_ShouldReturnFalse) {
    EXPECT_FALSE(dvm1->processPrepaidItem("INVALIDCODE"));
}

// TC-PRE-005: Save sale from other DVM
TEST_F(DVMTest, SaveSaleFromOther_ShouldRecordSaleAndDecreaseStock) {
    string cert_code = "SAVEFROMOTHER";
    EXPECT_NO_THROW(dvm1->saveSaleFromOther(item_coke.getItemCode(), 3, cert_code));
    
    // 재고 감소 확인: Coke 10개 -> 3개 판매 -> 7개 남음
    // 8개 주문 시도 시 에러 발생해야 함
    SaleRequest request_fail{item_coke.getItemCode(), 8, item_coke}; 
    EXPECT_THROW(dvm1->requestOrder(request_fail), runtime_error);
}

TEST_F(DVMTest, RequestOrder_ToNonExistentDVM_ShouldThrowDVMNotFoundException) {
    // dvm1의 otherDvms 리스트는 현재 비어있음.
    SaleRequest request{item_coke.getItemCode(), 1, item_coke};
    EXPECT_THROW(dvm1->requestOrder(999, request), DVMNotFoundException); // 존재하지 않는 DVM ID
}

// TC-PAY-001, TC-PAY-002, TC-PAY-003 테스트 케이스
// 이 테스트들은 DVM 클래스가 결제 처리를 하는 방식에 따라 달라질 수 있음
// 현재 DVM 클래스에는 직접적인 결제 메서드가 없으므로 requestOrder가 간접적으로 결제 처리를 한다고 가정

// TC-PAY-001: 유효한 카드로 결제 성공 (간접 테스트)
TEST_F(DVMTest, PaymentWithValidCard_ShouldSucceed) {
    SaleRequest request{item_coke.getItemCode(), 1, item_coke};
    // DVM이 결제 처리를 requestOrder 내부에서 한다고 가정
    EXPECT_NO_THROW(dvm1->requestOrder(request));
}

// TC-PAY-002: 잔액 부족으로 결제 실패 시나리오 (통합 테스트 필요)
// 이 테스트는 DVM 클래스가 Payment 시스템과 통합되어 있을 때 구현 가능
// 현재 구조에서는 requestOrder에서 결제 실패를 시뮬레이션하기 어려움

// TC-PAY-003: 유효하지 않은 카드로 결제 실패 시나리오 (통합 테스트 필요)
// 이 테스트도 Payment 시스템과의 통합이 필요

// TC-DIST-005: 동일한 거리의 VM 처리 (ID가 낮은 것 우선)
// 이 테스트는 OtherDVM 목킹의 한계로 인해 구현이 제한적이지만 시나리오 작성
TEST_F(DVMTest, SelectLowestIdDVMWhenEqualDistance) {
    // 이 테스트는 DVM이 거리가 같은 여러 DVM 중에서 ID가 가장 낮은 것을 선택하는지 확인
    // 현재 구조에서는 목킹 제한으로 인해 완전한 테스트가 어려움
    // 아래는 의사 코드로 테스트 시나리오만 표현
    
    /*
    // 두 개의 같은 거리의 DVM 설정
    MockOtherDVM* mock_dvm1 = new MockOtherDVM(101, Location(10, 10)); // ID 101
    MockOtherDVM* mock_dvm2 = new MockOtherDVM(102, Location(10, 10)); // ID 102, 같은 위치
    
    // 두 DVM 모두 재고가 있다고 가정
    ON_CALL(*mock_dvm1, findAvailableStocks(_))
        .WillByDefault(Return(CheckStockResponse{item_fanta.getItemCode(), 5, nullptr}));
    ON_CALL(*mock_dvm2, findAvailableStocks(_))
        .WillByDefault(Return(CheckStockResponse{item_fanta.getItemCode(), 3, nullptr}));
        
    // dvm1에 이 두 개의 OtherDVM을 알려줌 (현재 구조에서는 어려움)
    
    // 재고 조회
    string result = dvm1->queryStocks(item_fanta.getItemCode(), 1); // Fanta는 dvm1에 재고 없음
    
    // ID가 더 낮은 mock_dvm1(ID 101)의 정보가 반환되어야 함
    EXPECT_NE(result.find("flag:other"), string::npos);
    EXPECT_NE(result.find("x:10"), string::npos);
    EXPECT_NE(result.find("y:10"), string::npos);
    */
}

// TC-PRE-001: 선결제 옵션 제공 테스트
TEST_F(DVMTest, QueryStocks_ShouldProvidePrePaymentOption_WhenItemAvailableInOtherDVM) {
    // 이 테스트도 목킹 제한으로 완전한 테스트가 어려움
    // 아래는 의사 코드로 시나리오만 표현
    
    /*
    // 다른 DVM에 재고가 있는 상황 설정
    MockOtherDVM* mock_dvm = new MockOtherDVM(201, Location(20, 20));
    ON_CALL(*mock_dvm, findAvailableStocks(_))
        .WillByDefault(Return(CheckStockResponse{item_fanta.getItemCode(), 5, nullptr}));
        
    // dvm1에 이 OtherDVM을 알려줌 (현재 구조에서는 어려움)
    
    // 재고 조회 (dvm1에는 Fanta 재고 없음)
    string result = dvm1->queryStocks(item_fanta.getItemCode(), 1);
    
    // 다른 DVM의 정보가 반환되어야 함 (선결제 옵션 제공 의미)
    EXPECT_NE(result.find("flag:other"), string::npos);
    EXPECT_NE(result.find("x:20"), string::npos);
    EXPECT_NE(result.find("y:20"), string::npos);
    */
}

// TC-PRE-004: 재고가 있는 VM으로 안내 테스트
TEST_F(DVMTest, RequestOrder_ShouldReturnLocationAndCertCode) {
    // 이 테스트는 requestOrder(int targetDvmId, SaleRequest request)를 테스트
    // 목킹 제한으로 완전한 테스트가 어려움
    // 아래는 의사 코드로 시나리오만 표현
    
    /*
    // 다른 DVM 설정
    MockOtherDVM* mock_dvm = new MockOtherDVM(301, Location(30, 30));
    ON_CALL(*mock_dvm, askForPrepayment(_))
        .WillByDefault(Return(askPrepaymentResponse{true}));
        
    // dvm1에 이 OtherDVM을 알려줌
    
    SaleRequest request{item_coke.getItemCode(), 1, item_coke};
    pair<Location, string> result = dvm1->requestOrder(301, request);
    
    // 위치 정보와 인증 코드 확인
    EXPECT_EQ(result.first.getX(), 30);
    EXPECT_EQ(result.first.getY(), 30);
    EXPECT_FALSE(result.second.empty()); // 인증 코드가 생성됨
    */
}

// TC-COM-001: JSON 메시지 포맷 검증 (이 테스트는 DVM 클래스가 JSON 처리를 직접 한다면 가능)
// 현재 DVM 구현에서는 JSON 직접 처리가 보이지 않아 추가하지 않음

// TC-COM-002: 브로드캐스트 메시지 처리 (목킹 제한으로 테스트 어려움)

// TC-PERF-001, TC-PERF-002: 성능 테스트
// 이러한 성능 테스트는 단위 테스트보다는 통합/시스템 테스트에 더 적합
// 현재 프레임워크에서는 구현이 제한적

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 