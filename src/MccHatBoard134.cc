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

uint8_t MccHatBoard134::TcType(const string type) const
{
  if      (type == "J") return TC_TYPE_J;
  else if (type == "K") return TC_TYPE_K;
  else if (type == "T") return TC_TYPE_T;
  else if (type == "E") return TC_TYPE_E;
  else if (type == "R") return TC_TYPE_R;
  else if (type == "S") return TC_TYPE_S;
  else if (type == "B") return TC_TYPE_B;
  else if (type == "N") return TC_TYPE_N;
  else                  return TC_DISABLED;
}

std::string MccHatBoard134::TcType(const uint8_t type) const
{
  if      (type == TC_TYPE_J) return "J";
  else if (type == TC_TYPE_K) return "K";
  else if (type == TC_TYPE_T) return "T";
  else if (type == TC_TYPE_E) return "E";
  else if (type == TC_TYPE_R) return "R";
  else if (type == TC_TYPE_S) return "S";
  else if (type == TC_TYPE_B) return "B";
  else if (type == TC_TYPE_N) return "N";
  else                        return "?";
}

int MccHatBoard134::EnableChannel(const uint8_t chan, const string opt)
{
  return EnableChannel(chan, TcType(opt));
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

int MccHatBoard134::GetTcType(const uint8_t chan, std::string& type) const
{
  if (! IsValidChan(chan)) {
    type = "?";
    return 98;
  }
  uint8_t type2;
  int ret = mcc134_tc_type_read(m_addr, chan, &type2);
  type = TcType(type2);
  return ret;
}

int MccHatBoard134::SetTcType(const uint8_t chan, const std::string type)
{
  if (! IsValidChan(chan)) return 98;
  uint8_t type2 = TcType(type);
  return mcc134_tc_type_write(m_addr, chan, type2);
}

int MccHatBoard134::GetTcType(const std::string chan, std::string& type) const
{
  type = "";
  vector<uint8_t> list_chan;
  if (SplitChannelList(chan, list_chan) != 0) return 1;
  int ret = 0;
  for (auto it = list_chan.begin(); it != list_chan.end(); it++) {
    if (it != list_chan.begin()) type += "\t";
    string val;
    ret += GetTcType(*it, val);
    type += val;
  }
  return ret;
}

int MccHatBoard134::SetTcType(const std::string chan, const std::string type)
{
  vector<uint8_t> list_chan;
  if (SplitChannelList(chan, list_chan) != 0) return 1;
  int ret = 0;
  for (auto it = list_chan.begin(); it != list_chan.end(); it++) {
    ret += SetTcType(*it, type);
  }
  return ret;
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

