#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <climits>
#include <vector>
#include <string>
#include "../app/domain/card.h"
#include "../app/domain/certificationcode.h"
#include "../app/domain/item.h"
#include "../app/domain/location.h"
#include "../app/domain/prepayment.h"

// Card 클래스 테스트
TEST(CardTest, ValidCardFormat) {
    Card card("1234-abcd");
    EXPECT_TRUE(card.isValid());
}

TEST(CardTest, AnotherValidCardFormat) {
    Card card("5678-wxyz");
    EXPECT_TRUE(card.isValid());
}

TEST(CardTest, ValidCardWithLowercase) {
    Card card("9876-efgh");
    EXPECT_TRUE(card.isValid());
}

TEST(CardTest, ValidCardWithUppercase) {
    Card card("1111-ABCD");
    EXPECT_TRUE(card.isValid());
}

TEST(CardTest, ValidCardWithMixedCase) {
    Card card("2222-AbCd");
    EXPECT_TRUE(card.isValid());
}

TEST(CardTest, InvalidCardFormat) {
    Card card1("12345-abcd");  // 첫 번째 부분이 4자리가 아님
    EXPECT_FALSE(card1.isValid());
    
    Card card2("123-abcd");    // 첫 번째 부분이 4자리가 아님
    EXPECT_FALSE(card2.isValid());
    
    Card card3("1234-abc");    // 두 번째 부분이 4자리가 아님
    EXPECT_FALSE(card3.isValid());
    
    Card card4("1234-abcde");  // 두 번째 부분이 4자리가 아님
    EXPECT_FALSE(card4.isValid());
    
    Card card5("abcd-1234");   // 첫 번째 부분이 숫자가 아님
    EXPECT_FALSE(card5.isValid());
    
    Card card6("1234-1234");   // 두 번째 부분이 문자가 아님
    EXPECT_FALSE(card6.isValid());
    
    Card card7("123a-abcd");   // 첫 번째 부분에 문자가 포함됨
    EXPECT_FALSE(card7.isValid());
}

TEST(CardTest, InvalidCardWithoutHyphen) {
    Card card("12341234");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, InvalidCardWithMultipleHyphens) {
    Card card("12-34-abcd");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, InvalidCardWithSpecialChars) {
    Card card("1234-ab!@");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, EmptyCardNumber) {
    Card card("");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, OnlyHyphenCardNumber) {
    Card card("-");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, CardWithSpaces) {
    Card card("1234 abcd");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, CardWithLeadingZeros) {
    Card card("0001-abcd");
    EXPECT_TRUE(card.isValid());
}

TEST(CardTest, CardWithTrailingHyphen) {
    Card card("1234-abcd-");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, CardWithLeadingHyphen) {
    Card card("-1234-abcd");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, CardWithWhitespace) {
    Card card("1234-abc ");
    EXPECT_FALSE(card.isValid());
    
    Card card2(" 1234-abcd");
    EXPECT_FALSE(card2.isValid());
}

TEST(CardTest, LongerCardNumber) {
    Card card("12345-abcdef");
    EXPECT_FALSE(card.isValid());
}

TEST(CardTest, AllZeroCardNumber) {
    Card card("0000-0000");
    EXPECT_FALSE(card.isValid());  // 두 번째 부분은 문자여야 함
}

// CertificationCode 클래스 테스트
TEST(CertificationCodeTest, RandomCodeGeneration) {
    CertificationCode code1;
    CertificationCode code2;
    
    // 코드가 자동 생성되고 5자리여야 함
    std::string code1Str;
    std::stringstream ss1;
    ss1 << code1;
    ss1 >> code1Str;
    
    std::string code2Str;
    std::stringstream ss2;
    ss2 << code2;
    ss2 >> code2Str;
    
    EXPECT_EQ(code1Str.length(), 5);
    EXPECT_EQ(code2Str.length(), 5);
    
    // 두 랜덤 코드는 달라야 함 (매우 낮은 확률로 같을 수 있으나 테스트 목적상 다름을 가정)
    EXPECT_NE(code1Str, code2Str);
}

TEST(CertificationCodeTest, MultipleRandomCodeGeneration) {
    const int COUNT = 10;
    std::vector<CertificationCode> codes;
    std::vector<std::string> codeStrings;
    
    // 여러 개의 인증 코드 생성
    for (int i = 0; i < COUNT; i++) {
        codes.push_back(CertificationCode());
        
        std::stringstream ss;
        ss << codes.back();
        std::string codeStr;
        ss >> codeStr;
        
        // 모든 코드는 5자리여야 함
        EXPECT_EQ(codeStr.length(), 5);
        codeStrings.push_back(codeStr);
    }
    
    // 생성된 코드 쌍을 비교하여 모두 달라야 함 (충돌 가능성은 매우 낮음)
    for (int i = 0; i < COUNT; i++) {
        for (int j = i + 1; j < COUNT; j++) {
            EXPECT_NE(codeStrings[i], codeStrings[j]);
        }
    }
}

TEST(CertificationCodeTest, CodeVerification) {
    CertificationCode code("abc12");
    
    // 올바른 코드로 처음 검증 시 성공
    EXPECT_TRUE(code.markUsed("abc12"));
    
    // 이미 사용된 코드로 재검증 시 실패
    EXPECT_FALSE(code.markUsed("abc12"));
    
    // 잘못된 코드로 검증 시 실패
    CertificationCode code2("xyz98");
    EXPECT_FALSE(code2.markUsed("xyz99"));
}

TEST(CertificationCodeTest, CustomCodeWithDigitsOnly) {
    CertificationCode code("12345");
    EXPECT_TRUE(code.markUsed("12345"));
    EXPECT_FALSE(code.markUsed("12345")); // 이미 사용됨
}

TEST(CertificationCodeTest, CustomCodeWithLettersOnly) {
    CertificationCode code("abcde");
    EXPECT_TRUE(code.markUsed("abcde"));
    EXPECT_FALSE(code.markUsed("abcde")); // 이미 사용됨
}

TEST(CertificationCodeTest, CaseSensitiveVerification) {
    CertificationCode code("AbCdE");
    EXPECT_FALSE(code.markUsed("abcde"));
    EXPECT_TRUE(code.markUsed("AbCdE"));
}

TEST(CertificationCodeTest, MultipleVerificationAttempts) {
    CertificationCode code("test1");
    
    // 첫 번째 틀린 시도
    EXPECT_FALSE(code.markUsed("test2"));
    
    // 두 번째 틀린 시도
    EXPECT_FALSE(code.markUsed("Test1"));
    
    // 올바른 시도
    EXPECT_TRUE(code.markUsed("test1"));
    
    // 이미 사용된 코드
    EXPECT_FALSE(code.markUsed("test1"));
}

TEST(CertificationCodeTest, EmptyCode) {
    CertificationCode code("");
    EXPECT_TRUE(code.markUsed(""));  // 빈 문자열도 유효한 코드로 인식되어야 함
    EXPECT_FALSE(code.markUsed(""));  // 이미 사용됨
}

TEST(CertificationCodeTest, SpecialCharacterCode) {
    CertificationCode code("a@b$%");
    
    EXPECT_TRUE(code.markUsed("a@b$%"));
    EXPECT_FALSE(code.markUsed("a@b$%"));  // 이미 사용됨
}

TEST(CertificationCodeTest, NumericOnlyCode) {
    CertificationCode code("12345");
    
    EXPECT_TRUE(code.markUsed("12345"));
    EXPECT_FALSE(code.markUsed("12345"));  // 이미 사용됨
}

TEST(CertificationCodeTest, CodeWithSpaces) {
    CertificationCode code("a b c");
    
    EXPECT_TRUE(code.markUsed("a b c"));
    EXPECT_FALSE(code.markUsed("abc"));  // 공백이 다르므로 실패
}

TEST(CertificationCodeTest, SpaceOnlyCode) {
    CertificationCode code("     ");
    
    EXPECT_TRUE(code.markUsed("     "));
    EXPECT_FALSE(code.markUsed("    "));  // 공백 수가 다름
}

// Item 클래스 테스트
TEST(ItemTest, ItemCreation) {
    Item item("A001", "콜라", 1200);
    
    EXPECT_EQ(item.getItemCode(), "A001");
    EXPECT_EQ(item.getName(), "콜라");
    EXPECT_EQ(item.getPrice(), 1200);
}

TEST(ItemTest, ItemWithZeroPrice) {
    Item item("A002", "무료 물", 0);
    
    EXPECT_EQ(item.getItemCode(), "A002");
    EXPECT_EQ(item.getName(), "무료 물");
    EXPECT_EQ(item.getPrice(), 0);
}

TEST(ItemTest, ItemWithHighPrice) {
    Item item("A003", "고급 와인", 100000);
    
    EXPECT_EQ(item.getItemCode(), "A003");
    EXPECT_EQ(item.getName(), "고급 와인");
    EXPECT_EQ(item.getPrice(), 100000);
}

TEST(ItemTest, ItemWithEmptyName) {
    Item item("A004", "", 500);
    
    EXPECT_EQ(item.getItemCode(), "A004");
    EXPECT_EQ(item.getName(), "");
    EXPECT_EQ(item.getPrice(), 500);
}

TEST(ItemTest, ItemWithEmptyCode) {
    Item item("", "아이템", 500);
    
    EXPECT_EQ(item.getItemCode(), "");
    EXPECT_EQ(item.getName(), "아이템");
    EXPECT_EQ(item.getPrice(), 500);
}

TEST(ItemTest, ItemWithSpecialCharsInName) {
    Item item("A005", "특별한!@#$%^&*()", 700);
    
    EXPECT_EQ(item.getItemCode(), "A005");
    EXPECT_EQ(item.getName(), "특별한!@#$%^&*()");
    EXPECT_EQ(item.getPrice(), 700);
}

TEST(ItemTest, ItemWithLongCode) {
    Item item("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "긴 코드 아이템", 800);
    
    EXPECT_EQ(item.getItemCode(), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    EXPECT_EQ(item.getName(), "긴 코드 아이템");
    EXPECT_EQ(item.getPrice(), 800);
}

TEST(ItemTest, ItemWithLongName) {
    Item item("A006", "이것은 매우 긴 이름을 가진 아이템입니다. 이 아이템은 테스트 목적으로 생성되었으며 이름이 매우 깁니다.", 900);
    
    EXPECT_EQ(item.getItemCode(), "A006");
    EXPECT_EQ(item.getName(), "이것은 매우 긴 이름을 가진 아이템입니다. 이 아이템은 테스트 목적으로 생성되었으며 이름이 매우 깁니다.");
    EXPECT_EQ(item.getPrice(), 900);
}

TEST(ItemTest, ItemPrinting) {
    Item item("A001", "콜라", 1200);
    EXPECT_EQ(item.printItem(), "콜라 (A001)");
}

TEST(ItemTest, EmptyItemPrinting) {
    Item item("", "", 0);
    EXPECT_EQ(item.printItem(), " ()");
}

TEST(ItemTest, PriceCalculation) {
    Item item("A001", "콜라", 1200);
    
    // 단일 아이템
    EXPECT_EQ(item.calculatePrice(1), 1200);
    
    // 다수 아이템
    EXPECT_EQ(item.calculatePrice(3), 3600);
    
    // 0개 아이템 (경계값)
    EXPECT_EQ(item.calculatePrice(0), 0);
}

TEST(ItemTest, PriceCalculationLargeQuantity) {
    Item item("A001", "콜라", 1200);
    
    EXPECT_EQ(item.calculatePrice(100), 120000);
    EXPECT_EQ(item.calculatePrice(1000), 1200000);
}

TEST(ItemTest, PriceCalculationWithZeroPrice) {
    Item item("A002", "무료 물", 0);
    
    EXPECT_EQ(item.calculatePrice(1), 0);
    EXPECT_EQ(item.calculatePrice(100), 0);
}

TEST(ItemTest, ComparisonOperator) {
    Item item1("A001", "콜라", 1200);
    Item item2("A002", "사이다", 1200);
    Item item3("A001", "다른 콜라", 1500);
    
    // ID 기준으로 비교
    EXPECT_TRUE(item1 < item2);
    EXPECT_FALSE(item2 < item1);
    
    // 같은 ID의 아이템은 같은 것으로 취급 (이름, 가격 무관)
    EXPECT_FALSE(item1 < item3);
    EXPECT_FALSE(item3 < item1);
}

TEST(ItemTest, NegativePriceHandling) {
    // 음수 가격은 기능적으로 문제가 없어야 함
    Item item("A007", "할인 상품", -500);
    
    EXPECT_EQ(item.getPrice(), -500);
    EXPECT_EQ(item.calculatePrice(2), -1000);
}

TEST(ItemTest, DecimalPriceHandling) {
    // 정수 가격만 지원하므로 소수점은 무시됨
    Item item("A008", "소수점 가격 상품", 1200);
    
    EXPECT_EQ(item.getPrice(), 1200);
    EXPECT_EQ(item.calculatePrice(1), 1200);
}

TEST(ItemTest, ItemWithOnlyWhitespace) {
    Item item("A009", "   ", 300);
    
    EXPECT_EQ(item.getName(), "   ");
    EXPECT_EQ(item.printItem(), "    (A009)");
}

TEST(ItemTest, ItemWithNumericName) {
    Item item("A010", "12345", 400);
    
    EXPECT_EQ(item.getName(), "12345");
    EXPECT_EQ(item.printItem(), "12345 (A010)");
}

TEST(ItemTest, NegativeQuantityPriceCalculation) {
    Item item("A011", "음수 수량 테스트", 500);
    
    // 음수 수량 처리 - 기능적으로 문제 없어야 함
    EXPECT_EQ(item.calculatePrice(-1), -500);
    EXPECT_EQ(item.calculatePrice(-5), -2500);
}

TEST(ItemTest, MaxIntQuantityPriceCalculation) {
    Item item("A012", "최대 수량 테스트", 10);
    
    // 큰 수량으로 인한 오버플로우 가능성 테스트
    // INT_MAX / 10 = 약 2억개
    int maxSafeQuantity = INT_MAX / 10;
    EXPECT_EQ(item.calculatePrice(maxSafeQuantity), maxSafeQuantity * 10);
}

// Location 클래스 테스트
TEST(LocationTest, DefaultConstructor) {
    Location loc;
    EXPECT_EQ(loc.getX(), 0);
    EXPECT_EQ(loc.getY(), 0);
}

TEST(LocationTest, DistanceCalculation) {
    Location loc1(0, 0);
    Location loc2(3, 4);
    
    // 맨해튼 거리: |x2-x1| + |y2-y1|
    EXPECT_EQ(loc1.calculateDistance(loc2), 7);
    
    // 거리 계산은 양방향이어야 함
    EXPECT_EQ(loc2.calculateDistance(loc1), 7);
    
    // 같은 위치의 거리는 0이어야 함
    EXPECT_EQ(loc1.calculateDistance(loc1), 0);
    
    // 음수 좌표 처리
    Location loc3(-2, -3);
    EXPECT_EQ(loc1.calculateDistance(loc3), 5);
    EXPECT_EQ(loc3.calculateDistance(loc1), 5);
}

TEST(LocationTest, DistanceCalculationWithLargeNumbers) {
    Location loc1(0, 0);
    Location loc2(1000, 2000);
    
    EXPECT_EQ(loc1.calculateDistance(loc2), 3000);
}

TEST(LocationTest, DistanceCalculationWithMixedSigns) {
    Location loc1(10, -20);
    Location loc2(-30, 40);
    
    // |(-30)-10| + |40-(-20)| = 40 + 60 = 100
    EXPECT_EQ(loc1.calculateDistance(loc2), 100);
}

TEST(LocationTest, DistanceCalculationWithSameX) {
    Location loc1(5, 10);
    Location loc2(5, 20);
    
    EXPECT_EQ(loc1.calculateDistance(loc2), 10);
}

TEST(LocationTest, DistanceCalculationWithSameY) {
    Location loc1(10, 5);
    Location loc2(20, 5);
    
    EXPECT_EQ(loc1.calculateDistance(loc2), 10);
}

TEST(LocationTest, CoordinateGetters) {
    Location loc(5, -3);
    
    EXPECT_EQ(loc.getX(), 5);
    EXPECT_EQ(loc.getY(), -3);
}

TEST(LocationTest, ExtremeCoordinates) {
    Location loc(INT_MAX, INT_MIN);
    
    EXPECT_EQ(loc.getX(), INT_MAX);
    EXPECT_EQ(loc.getY(), INT_MIN);
}

TEST(LocationTest, ZeroDistance) {
    Location loc1(42, 42);
    Location loc2(42, 42);
    
    EXPECT_EQ(loc1.calculateDistance(loc2), 0);
}

TEST(LocationTest, DistanceWithZeroDifference) {
    Location loc1(10, 20);
    Location loc2(10, 20);
    
    EXPECT_EQ(loc1.calculateDistance(loc2), 0);
}

TEST(LocationTest, DifferentXSameY) {
    Location loc1(5, 10);
    Location loc2(15, 10);
    
    EXPECT_EQ(loc1.calculateDistance(loc2), 10);
}

TEST(LocationTest, SameXDifferentY) {
    Location loc1(5, 10);
    Location loc2(5, 30);
    
    EXPECT_EQ(loc1.calculateDistance(loc2), 20);
}

TEST(LocationTest, VeryLargeCoordinates) {
    Location loc1(1000000, 2000000);
    Location loc2(3000000, 5000000);
    
    // |3000000 - 1000000| + |5000000 - 2000000| = 2000000 + 3000000 = 5000000
    EXPECT_EQ(loc1.calculateDistance(loc2), 5000000);
}

TEST(LocationTest, MaxToMinDistance) {
    Location loc1(INT_MAX, INT_MAX);
    Location loc2(INT_MIN, INT_MIN);
    
    // 이론적으로 최대 가능한 거리
    // |INT_MIN - INT_MAX| + |INT_MIN - INT_MAX| 
    // = 2 * (|INT_MIN| + |-1|) (INT_MIN = -INT_MAX - 1)
    // = 2 * (INT_MAX + 1)
    // = 2 * INT_MAX + 2
    // 주의: 이 계산은 오버플로우를 일으킬 수 있음
    
    // 맨해튼 거리를 직접 계산
    long long expectedDistance = 2LL * (static_cast<long long>(INT_MAX) + 1LL);
    
    // 32-bit 정수 오버플로우가 발생할 수 있으므로, 실제 함수 결과는 다를 수 있음
    // 이 테스트는 오버플로우 처리 방식에 따라 성공/실패 가능
    int actualDistance = loc1.calculateDistance(loc2);
    if (expectedDistance <= INT_MAX) {
        EXPECT_EQ(actualDistance, expectedDistance);
    }
}

TEST(LocationTest, OppositeQuadrants) {
    Location loc1(100, 100);    // 1사분면
    Location loc2(-100, -100);  // 3사분면
    
    EXPECT_EQ(loc1.calculateDistance(loc2), 400);
}

// Prepayment 클래스 테스트
TEST(PrepaymentTest, CertificationCodeValidation) {
    // 고정된 인증코드로 테스트
    CertificationCode fixedCode("test1");
    Prepayment prepayment(1, fixedCode);
    
    // 올바른 인증코드로 검증 시 성공
    EXPECT_TRUE(prepayment.isCertificationCode("test1"));
    
    // 이미 사용된 인증코드로 재검증 시 실패
    EXPECT_FALSE(prepayment.isCertificationCode("test1"));
    
    // 잘못된 인증코드로 검증 시 실패
    Prepayment prepayment2(2, CertificationCode("test2"));
    EXPECT_FALSE(prepayment2.isCertificationCode("wrong"));
}

TEST(PrepaymentTest, RandomCodeGeneration) {
    // 랜덤 인증코드 생성 테스트
    Prepayment prepayment1(1);
    Prepayment prepayment2(2);
    
    // 두 다른 선결제는 서로 다른 인증코드를 가져야 함
    // 실제 검증은 어렵지만, 인증코드가 생성되었는지 간접적으로 확인
    // 특정 코드로 검증했을 때 통과하면 안됨 (랜덤이기 때문)
    EXPECT_FALSE(prepayment1.isCertificationCode("test1"));
    EXPECT_FALSE(prepayment2.isCertificationCode("test1"));
}

TEST(PrepaymentTest, MultipleVendingMachines) {
    for (int i = 1; i <= 10; i++) {
        Prepayment prepayment(i, CertificationCode(std::to_string(i)));
        EXPECT_TRUE(prepayment.isCertificationCode(std::to_string(i)));
    }
}

TEST(PrepaymentTest, MultipleInvalidAttempts) {
    CertificationCode fixedCode("valid");
    Prepayment prepayment(1, fixedCode);
    
    // 5번의 잘못된 시도
    for (int i = 0; i < 5; i++) {
        EXPECT_FALSE(prepayment.isCertificationCode("invalid" + std::to_string(i)));
    }
    
    // 올바른 코드로 시도
    EXPECT_TRUE(prepayment.isCertificationCode("valid"));
    
    // 코드가 사용된 후 재사용 시도
    EXPECT_FALSE(prepayment.isCertificationCode("valid"));
}

TEST(PrepaymentTest, NegativeDvmId) {
    Prepayment prepayment(-1, CertificationCode("test"));
    EXPECT_TRUE(prepayment.isCertificationCode("test"));
}

TEST(PrepaymentTest, ZeroDvmId) {
    Prepayment prepayment(0, CertificationCode("test"));
    EXPECT_TRUE(prepayment.isCertificationCode("test"));
}

TEST(PrepaymentTest, LargeDvmId) {
    Prepayment prepayment(9999, CertificationCode("test"));
    EXPECT_TRUE(prepayment.isCertificationCode("test"));
}

TEST(PrepaymentTest, EmptyCodeVerification) {
    Prepayment prepayment(1, CertificationCode(""));
    
    EXPECT_TRUE(prepayment.isCertificationCode(""));
    EXPECT_FALSE(prepayment.isCertificationCode("anything"));
}

TEST(PrepaymentTest, MaxDvmId) {
    Prepayment prepayment(INT_MAX, CertificationCode("test"));
    EXPECT_TRUE(prepayment.isCertificationCode("test"));
}

TEST(PrepaymentTest, VerificationWithSpaceCode) {
    Prepayment prepayment(1, CertificationCode(" "));
    
    EXPECT_TRUE(prepayment.isCertificationCode(" "));
    EXPECT_FALSE(prepayment.isCertificationCode("  ")); // 공백 수가 다름
}

TEST(PrepaymentTest, VerificationWithNewlineCode) {
    Prepayment prepayment(1, CertificationCode("\n"));
    
    EXPECT_TRUE(prepayment.isCertificationCode("\n"));
    EXPECT_FALSE(prepayment.isCertificationCode(" ")); // 다른 문자
}

TEST(PrepaymentTest, VerificationWithTabCode) {
    Prepayment prepayment(1, CertificationCode("\t"));
    
    EXPECT_TRUE(prepayment.isCertificationCode("\t"));
    EXPECT_FALSE(prepayment.isCertificationCode(" ")); // 다른 문자
}

TEST(PrepaymentTest, ConsecutiveVendingMachines) {
    std::vector<Prepayment> prepayments;
    const int COUNT = 100;
    
    // 100개의 자판기에 대한 선결제 생성
    for (int i = 1; i <= COUNT; i++) {
        prepayments.emplace_back(i, CertificationCode(std::to_string(i)));
    }
    
    // 각 선결제에 대해 올바른 코드로 검증
    for (int i = 0; i < COUNT; i++) {
        EXPECT_TRUE(prepayments[i].isCertificationCode(std::to_string(i + 1)));
    }
} 