#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include "MccHatBoard134.h"
using namespace std;

MccHatBoard134::MccHatBoard134(const uint8_t addr)
  : MccHatBoard(addr)
{
  memset(m_tc_type, TC_DISABLED, sizeof(m_tc_type));
}

MccHatBoard134::~MccHatBoard134()
{
  ;
}

int MccHatBoard134::EnableChannel(const uint8_t chan, const string opt)
{
  uint8_t tc_type = 0;
  if      (opt == "J") tc_type = TC_TYPE_J;
  else if (opt == "K") tc_type = TC_TYPE_K;
  else if (opt == "T") tc_type = TC_TYPE_T;
  else if (opt == "E") tc_type = TC_TYPE_E;
  else if (opt == "R") tc_type = TC_TYPE_R;
  else if (opt == "S") tc_type = TC_TYPE_S;
  else if (opt == "B") tc_type = TC_TYPE_B;
  else if (opt == "N") tc_type = TC_TYPE_N;
  else                 tc_type = TC_DISABLED;
  return EnableChannel(chan, tc_type);
}

int MccHatBoard134::EnableChannelListed(const std::string list_chan, const std::string opt)
{
  int ret = 0;
  vector<uint8_t> list_chan2;
  if (SplitChannelList(list_chan, list_chan2) != 0) return 1;
  for (auto it = list_chan2.begin(); it != list_chan2.end(); it++) ret += EnableChannel(*it, opt);
  return ret;
}

int MccHatBoard134::EnableChannel(const uint8_t chan, const uint8_t tc_type)
{
  if (! IsValidChan(chan)) return 1;
  if (tc_type == TC_DISABLED) return 2;
  m_enabled[chan] = true;
  m_tc_type[chan] = tc_type;
  return mcc134_tc_type_write(m_addr, chan, tc_type);
}

int MccHatBoard134::EnableChannelListed(const std::string list_chan, const uint8_t tc_type)
{
  int ret = 0;
  vector<uint8_t> list_chan2;
  if (SplitChannelList(list_chan, list_chan2) != 0) return 1;
  for (auto it = list_chan2.begin(); it != list_chan2.end(); it++) ret += EnableChannel(*it, tc_type);
  return ret;
}

int MccHatBoard134::DisableChannel(const uint8_t chan)
{
  if (! IsValidChan(chan)) return 1;
  m_enabled[chan] = false;
  m_tc_type[chan] = TC_DISABLED;
  return mcc134_tc_type_write(m_addr, chan, TC_DISABLED);
}

int MccHatBoard134::ReadChannel(const uint8_t chan, double& value)
{
  if (! IsValidChan(chan)) return 1;
  if (m_tc_type[chan] == TC_DISABLED) {
    value = 0;
    return 2;
  }

  int ret = mcc134_t_in_read(m_addr, chan, &value);
  if (ret != 0) return ret;

  if      (value == OPEN_TC_VALUE) return 11;
  else if (value == OVERRANGE_TC_VALUE) return 12;
  else if (value == COMMON_MODE_TC_VALUE) return 13;
  return 0;
}

int MccHatBoard134::ReadEnabled(std::map<uint8_t, double>& values)
{
  int ret = 0;
  values.clear();
  for (uint8_t chan = 0; chan < N_CHAN; chan++) {
    if (m_tc_type[chan] == TC_DISABLED) continue;
    double value;
    ret += ReadChannel(chan, value);
    values[chan] = value;
  }
  return ret;
}

int MccHatBoard134::ReadAll(std::vector<double>& values)
{
  int ret = 0;
  values.clear();
  for (uint8_t chan = 0; chan < N_CHAN; chan++) {
    double value = 0;
    if (m_enabled[chan]) ret += ReadChannel(chan, value);
    values.push_back(value);
  }
  return ret;
}

