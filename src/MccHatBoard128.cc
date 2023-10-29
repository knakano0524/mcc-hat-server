#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include "MccHatBoard128.h"
using namespace std;

MccHatBoard128::MccHatBoard128(const uint8_t addr)
  : MccHatBoard(addr)
{
  memset(m_opts   , 0, sizeof(m_opts   ));
}

MccHatBoard128::~MccHatBoard128()
{
  ;
}

int MccHatBoard128::EnableChannel(const uint8_t chan, const string opt)
{
  uint32_t opt2 = 0;
  if (opt.find("noscale") != string::npos) opt2 |= OPTS_NOSCALEDATA;
  if (opt.find("nocalib") != string::npos) opt2 |= OPTS_NOCALIBRATEDATA;
  return EnableChannel(chan, opt2);
}

int MccHatBoard128::EnableChannelListed(const std::string list_chan, const string opt)
{
  vector<uint8_t> list_chan2;
  if (SplitChannelList(list_chan, list_chan2) != 0) return 1;
  for (auto it = list_chan2.begin(); it != list_chan2.end(); it++) EnableChannel(*it, opt);
  return 0;
}

int MccHatBoard128::EnableChannel(const uint8_t chan, const uint32_t opt)
{  
  if (! IsValidChan(chan)) return 1;
  m_enabled[chan] = true;
  m_opts   [chan] = opt;
  return 0;
}

int MccHatBoard128::EnableChannelListed(const std::string list_chan, const uint32_t opt)
{
  vector<uint8_t> list_chan2;
  if (SplitChannelList(list_chan, list_chan2) != 0) return 1;
  for (auto it = list_chan2.begin(); it != list_chan2.end(); it++) EnableChannel(*it, opt);
  return 0;
}

int MccHatBoard128::DisableChannel(const uint8_t chan)
{
  if (! IsValidChan(chan)) return 1;
  m_enabled[chan] = false;
  m_opts   [chan] = 0;
  return 0;
}

int MccHatBoard128::SetInputMode(const std::string mode)
{
  uint8_t mode2;
  if      (mode == "SE"  ) mode2 = A_IN_MODE_SE;
  else if (mode == "DIFF") mode2 = A_IN_MODE_DIFF;
  else return 1;
  return mcc128_a_in_mode_write(m_addr, mode2);
}

int MccHatBoard128::SetInputRange(const std::string range)
{
  uint8_t range2;
  if      (range == "10V") range2 = A_IN_RANGE_BIP_10V;
  else if (range ==  "5V") range2 = A_IN_RANGE_BIP_5V;
  else if (range ==  "2V") range2 = A_IN_RANGE_BIP_2V;
  else if (range ==  "1V") range2 = A_IN_RANGE_BIP_1V;
  else return 1;
  return mcc128_a_in_range_write(m_addr, range2);
}

int MccHatBoard128::ReadChannel(const uint8_t chan, double& value)
{
  if (! IsValidChan(chan)) return 1;
  if (! m_enabled[chan]  ) return 2;
  return mcc128_a_in_read(m_addr, chan, m_opts[chan], &value);
}

int MccHatBoard128::ReadEnabled(std::map<uint8_t, double>& values)
{
  int ret = 0;
  values.clear();
  for (uint8_t chan = 0; chan < N_CHAN; chan++) {
    if (! m_enabled[chan]) continue;
    double value;
    ret += ReadChannel(chan, value);
    values[chan] = value;
  }
  return ret;
}

int MccHatBoard128::ReadAll(std::vector<double>& values)
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
