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

int MccHatBoard::EnableChannel(const uint8_t chan)
{
  if (! IsValidChan(chan)) return 98;
  m_enabled[chan] = true;
  return 0;
}

int MccHatBoard::EnableChannel(const std::string chan)
{
  vector<uint8_t> chan2;
  if (SplitChannelList(chan, chan2) != 0) return 98;
  for (auto it = chan2.begin(); it != chan2.end(); it++) m_enabled[*it] = true;
  return 0;
}

int MccHatBoard::DisableChannel(const uint8_t chan)
{
  if (! IsValidChan(chan)) return 98;
  m_enabled[chan] = false;
  return 0;
}

int MccHatBoard::DisableChannel(const std::string chan)
{
  vector<uint8_t> chan2;
  if (SplitChannelList(chan, chan2) != 0) return 98;
  for (auto it = chan2.begin(); it != chan2.end(); it++) m_enabled[*it] = false;
  return 0;
}

int MccHatBoard::SplitChannelList(const std::string list_chan_str, std::vector<uint8_t>& list_chan) const
{
  list_chan.clear();
  if (list_chan_str == "all") {
    for (int ch = 0; ch < GetNumChan(); ch++) list_chan.push_back(ch);
    return 0;
  }
  
  vector<string> list_ele; // "1", "2-3", etc.
  size_t last = 0, next;
  while ((next = list_chan_str.find(",", last)) != string::npos) {
    list_ele.push_back( list_chan_str.substr(last, next-last) );
    last = next + 1;
  }
  if (last < list_chan_str.length()) list_ele.push_back( list_chan_str.substr(last) );

  for (auto it = list_ele.begin(); it != list_ele.end();it++) {
    size_t pos = it->find("-");
    if (pos != string::npos) {
      int ch0 = atoi( it->substr(0, pos).c_str() );
      int ch1 = atoi( it->substr(pos+1 ).c_str() );
      for (int ch = ch0; ch <= ch1; ch++) list_chan.push_back(ch);
    } else {
      list_chan.push_back(atoi(it->c_str()));
    }
  }
  return 0;
}
