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

int MccHatBoard128::GetChannelOpts(const uint8_t chan, std::string& opts) const
{
  if (! IsValidChan(chan)) {
    opts = "NG";
    return 98;
  }
  if (m_opts[chan] == 0)   opts = "None";
  else {
    opts = "";
    if (m_opts[chan] & OPTS_NOSCALEDATA    ) opts += "NoScale";
    if (m_opts[chan] & OPTS_NOCALIBRATEDATA) opts += "NoCalib";
  }
  return 0;
}

int MccHatBoard128::SetChannelOpts(const uint8_t chan, const std::string opts)
{
  cout << "Z " << (int)m_addr << " " << (int)chan << " " << opts << endl;
  if (! IsValidChan(chan)) return 98;
  m_opts[chan] = 0;
  if (opts.find("noscale") != string::npos) m_opts[chan] |= OPTS_NOSCALEDATA;
  if (opts.find("nocalib") != string::npos) m_opts[chan] |= OPTS_NOCALIBRATEDATA;
  return 0;
}

int MccHatBoard128::GetChannelOpts(const std::string chan, std::string& opts) const
{
  opts = "";
  vector<uint8_t> list_chan;
  if (SplitChannelList(chan, list_chan) != 0) return 98;
  int ret = 0;
  for (auto it = list_chan.begin(); it != list_chan.end(); it++) {
    if (it != list_chan.begin()) opts += "\t";
    string val;
    ret += GetChannelOpts(*it, val);
    opts += val;
  }
  return ret;
}

int MccHatBoard128::SetChannelOpts(const std::string chan, const std::string opts)
{
  vector<uint8_t> list_chan;
  if (SplitChannelList(chan, list_chan) != 0) return 98;
  int ret = 0;
  for (auto it = list_chan.begin(); it != list_chan.end(); it++) {
    ret += SetChannelOpts(*it, opts);
  }
  return ret;
}

int MccHatBoard128::GetInputMode(std::string& mode) const
{
  uint8_t mode2;
  int ret = mcc128_a_in_mode_read(m_addr, &mode2);
  if (ret == RESULT_SUCCESS) {
    if      (mode2 == A_IN_MODE_SE  ) mode = "SE"  ;
    else if (mode2 == A_IN_MODE_DIFF) mode = "DIFF";
    else                              mode = "??";
  } else mode = "NG";
  return ret;
}

int MccHatBoard128::SetInputMode(const std::string mode)
{
  uint8_t mode2;
  if      (mode == "SE"  ) mode2 = A_IN_MODE_SE;
  else if (mode == "DIFF") mode2 = A_IN_MODE_DIFF;
  else return 1;
  return mcc128_a_in_mode_write(m_addr, mode2);
}

int MccHatBoard128::GetInputRange(std::string& range) const
{
  uint8_t range2;
  int ret = mcc128_a_in_range_read(m_addr, &range2);
  if (ret == RESULT_SUCCESS) {
    if      (range2 == A_IN_RANGE_BIP_10V) range = "10V";
    else if (range2 == A_IN_RANGE_BIP_5V ) range =  "5V";
    else if (range2 == A_IN_RANGE_BIP_2V ) range =  "2V";
    else if (range2 == A_IN_RANGE_BIP_1V ) range =  "1V";
    else                                   range =  "??";
  } else range = "NG";
  return ret;
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
