#include <iostream>
#include <thread>
#include "application/otherdvm.h"
#include "application/sale.h"
#include "application/dvm.h"
#include "presentation/controller.h"
#include <list>
#include <map>

using namespace std;

// IP:PORT 문자열을 파싱하는 함수
pair<string, int> parseIpPort(const string& ipPort) {
    size_t colonPos = ipPort.find(':');
    if (colonPos == string::npos) {
        throw invalid_argument("IP:PORT 형식이 아닙니다: " + ipPort);
    }
    
    string ip = ipPort.substr(0, colonPos);
    string portStr = ipPort.substr(colonPos + 1);
    
    if (ip.empty() || portStr.empty()) {
        throw invalid_argument("IP 또는 PORT가 비어있습니다: " + ipPort);
    }
    
    int port = stoi(portStr);
    if (port <= 0 || port > 65535) {
        throw invalid_argument("포트 번호가 잘못되었습니다: " + portStr);
    }
    
    return make_pair(ip, port);
}

int main(int argc, char* argv[])
{
  // 내 자판기 포트 하드코딩
  const int myPort = 9000;
  
  Config::get().setPort(myPort);
  Location loc(1, 2);

  // OtherDVM 리스트 - 명령행 인수에 따라 결정
  list<OtherDVM> otherDvms;
  
  // argc >= 2부터 다른 DVM 정보들을 파싱
  for (int i = 1; i < argc; i++) {
    try {
      auto [ip, port] = parseIpPort(argv[i]);
      
      // 임시로 위치는 (i*2, i*2)로 설정
      otherDvms.emplace_back(i+1, Location(i*2, i*2), ip.c_str(), port);
    } catch (const exception& e) {
      cerr << "오류: " << argv[i] << " - " << e.what() << endl;
      return 1;
    }
  }

  // 더미 Item 리스트
  std::list<Item> itemList;
  itemList.emplace_back("01", "콜라", 1500);
  itemList.emplace_back("02", "사이다", 1400);

  // 더미 stockList
  std::map<Item, int> stockList;
  for (const auto& item : itemList) {
      stockList[item] = 10;
  }

  // 빈 Sale 리스트
  std::list<Sale> saleList;

  DVM dvm(1, loc, stockList, itemList, saleList, otherDvms);
  Controller controller(&dvm);

  // controller 실행
  std::thread serverThread(&Controller::runServer, &controller);
  controller.run();
  serverThread.join();

  return 0;
}
