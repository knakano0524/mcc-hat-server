#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include "MccHatBoard118.h"
using namespace std;

MccHatBoard118::MccHatBoard118(const uint8_t addr)
  : MccHatBoard(addr)
{
  memset(m_opts, 0, sizeof(m_opts));
}

MccHatBoard118::~MccHatBoard118()
{
  ;
}

int MccHatBoard118::GetChannelOpts(const uint8_t chan, std::string& opts) const
{
  if (! IsValidChan(chan)) {
    opts = "NG";
    return 98;
  }
  if (m_opts[chan] == 0) opts = "None";
  else {
    opts = "";
    if (m_opts[chan] & OPTS_NOSCALEDATA    ) opts += "NoScale";
    if (m_opts[chan] & OPTS_NOCALIBRATEDATA) opts += "NoCalib";
  }
  return 0;
}

int MccHatBoard118::SetChannelOpts(const uint8_t chan, const std::string opts)
{
  if (! IsValidChan(chan)) return 98;
  m_opts[chan] = 0;
  if (opts.find("noscale") != string::npos) m_opts[chan] |= OPTS_NOSCALEDATA;
  if (opts.find("nocalib") != string::npos) m_opts[chan] |= OPTS_NOCALIBRATEDATA;
  return 0;
}

int MccHatBoard118::GetChannelOpts(const std::string chan, std::string& opts) const
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

int MccHatBoard118::SetChannelOpts(const std::string chan, const std::string opts)
{
  vector<uint8_t> list_chan;
  if (SplitChannelList(chan, list_chan) != 0) return 98;
  int ret = 0;
  for (auto it = list_chan.begin(); it != list_chan.end(); it++) {
    ret += SetChannelOpts(*it, opts);
  }
  return ret;
}

int MccHatBoard118::ReadChannel(const uint8_t chan, double& value)
{
  if (! IsValidChan(chan)) return 1;
  if (! m_enabled[chan]  ) return 2;
  return mcc118_a_in_read(m_addr, chan, m_opts[chan], &value);
}

int MccHatBoard118::ReadEnabled(std::map<uint8_t, double>& values)
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

int MccHatBoard118::ReadAll(std::vector<double>& values)
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
