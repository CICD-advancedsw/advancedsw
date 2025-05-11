#include "dvm.h"
#include <iostream>
#include <vector>

using namespace std;

DVM::DVM() : dvmId(0), location(Location()), stocks(), items(), sales(), dvms()
{
  initializeItems();
  initializeStocks();
}

DVM::DVM(int id, Location loc, std::list<OtherDVM> otherDvms) : dvmId(id), location(loc), dvms(otherDvms)
{
  initializeItems();
  initializeStocks();
}

void DVM::initializeItems()
{
  std::vector<std::pair<std::string, std::string>> itemData = {
      {"01", "콜라"}, {"02", "사이다"}, {"03", "녹차"}, {"04", "홍차"}, {"05", "밀크티"}, {"06", "탄산수"}, {"07", "보리차"}, {"08", "캔커피"}, {"09", "물"}, {"10", "에너지드링크"}, {"11", "유자차"}, {"12", "식혜"}, {"13", "아이스티"}, {"14", "딸기주스"}, {"15", "오렌지주스"}, {"16", "포도주스"}, {"17", "이온음료"}, {"18", "아메리카노"}, {"19", "핫초코"}, {"20", "카페라떼"}};

  for (const auto &data : itemData)
  {
    items.push_back(Item(data.first, data.second, 1000));
  }
}

void DVM::initializeStocks()
{
  for (const auto &item : items)
  {
    stocks[item] = 10; // 기본 재고 10개
  }
}

string DVM::queryItems()
{
  string result;
  int count = 0;

  for (const auto &item : items)
  {
    result += item.printItem();
    count++;
    if (count % 4 == 0)
    {
      result += "\n";
    }
    else
    {
      result += "\t";
    }
  }

  return result;
}

Item DVM::findItemByCode(const std::string &itemCode)
{
  for (const auto &item : items)
  {

    if (item.printItem().find(itemCode) != std::string::npos)
    {
      return item;
    }
  }
  throw std::invalid_argument("Invalid item code");
}

string DVM::queryStocks(const std::string itemCode, int count)
{
  try
  {
    Item item = findItemByCode(itemCode);
    if (stocks[item] < count)
    {
      return "재고 없음";
    }
    int price = item.calculatePrice(count);
    return "음료 가격 총 " + to_string(price) + "원 (" + itemCode + " 1개 " + to_string(item.calculatePrice(1)) + "원 * " + to_string(count) + ")";
  }
  catch (const std::invalid_argument &e)
  {
    return "Invalid error";
  }
}

void DVM::requestOrder()
{
  // todo: 주문 요청 처리 및 카드결제 로직 추가
}

string DVM::processPrepaidItem(const std::string certCode)
{
  // todo: 선결제 아이템 처리 로직 추가
  return "";
}