#ifndef __MCC_HAT_BOARD_118_H__
#define __MCC_HAT_BOARD_118_H__
#include "MccHatBoard.h"

/// Class to represent MCC HAT118.
class MccHatBoard118 : public MccHatBoard {
  static const uint8_t N_CHAN = 8;
  uint8_t m_opts[N_CHAN];
    
public:
  MccHatBoard118(const uint8_t addr);
  virtual ~MccHatBoard118();

  uint8_t GetNumChan() const { return N_CHAN; }
  
  int  Open()   { return mcc118_open (m_addr) == RESULT_SUCCESS; }
  int  Close()  { return mcc118_close(m_addr) == RESULT_SUCCESS; }
  bool IsOpen() { return mcc118_is_open(m_addr) == 1; }

  int GetChannelOpts(const uint8_t chan, std::string& opts) const;
  int SetChannelOpts(const uint8_t chan, const std::string opts);
  int GetChannelOpts(const std::string chan, std::string& opts) const;
  int SetChannelOpts(const std::string chan, const std::string opts);
  
  int ReadChannel(const uint8_t chan, double& value);
  int ReadEnabled(std::map<uint8_t, double>& values);
  int ReadAll(std::vector<double>& values);
};

#endif // __MCC_HAT_BOARD_118_H__
