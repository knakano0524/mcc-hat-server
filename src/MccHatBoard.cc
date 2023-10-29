#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include "MccHatBoard.h"
using namespace std;

MccHatBoard::MccHatBoard(const uint8_t addr)
  : m_verb(0)
  , m_addr(addr)
{
  memset(m_enabled, 0, sizeof(m_enabled));
}

MccHatBoard::~MccHatBoard()
{
  ;
}

int MccHatBoard::SplitChannelList(const std::string list_chan_str, std::vector<uint8_t>& list_chan)
{
  vector<string> list_ele; // "1", "2-3", etc.
  size_t last = 0, next;
  while ((next = list_chan_str.find(",", last)) != string::npos) {
    list_ele.push_back( list_chan_str.substr(last, next-last) );
    last = next + 1;
  }
  if (last < list_chan_str.length()) list_ele.push_back( list_chan_str.substr(last) );

  list_chan.clear();
  for (auto it = list_ele.begin(); it != list_ele.end();it++) {
    size_t pos = it->find("-");
    if (pos != string::npos) {
      int ch0 = atoi( it->substr(0, pos-1).c_str() );
      int ch1 = atoi( it->substr(pos+1   ).c_str() );
      for (int ch = ch0; ch <= ch1; ch++) list_chan.push_back(ch);
    } else {
      list_chan.push_back(atoi(it->c_str()));
    }
  }
  return 0;
}
