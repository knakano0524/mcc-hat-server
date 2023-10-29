#ifndef __MCC_HAT_BOARD_128_H__
#define __MCC_HAT_BOARD_128_H__
#include "MccHatBoard.h"

/// Class to represent MCC HAT128.
class MccHatBoard128 : public MccHatBoard {
  static const uint8_t N_CHAN = 8;
  uint8_t m_opts[N_CHAN];
    
public:
  MccHatBoard128(const uint8_t addr);
  virtual ~MccHatBoard128();

  uint8_t GetNumChan() const { return N_CHAN; }
  
  int  Open()   { return mcc128_open (m_addr) == RESULT_SUCCESS; }
  int  Close()  { return mcc128_close(m_addr) == RESULT_SUCCESS; }
  bool IsOpen() { return mcc128_is_open(m_addr) == 1; }

  int EnableChannel(const uint8_t chan, const std::string opt="");
  int EnableChannelListed(const std::string list_chan, const std::string opt="");

  int EnableChannel(const uint8_t chan, const uint32_t opt); // noscale,nocalib
  int EnableChannelListed(const std::string list_chan, const uint32_t opt);

  int DisableChannel(const uint8_t chan);
  
  int SetInputMode(const std::string mode); // SE, DIFF
  int SetInputRange(const std::string range); // 10V, 5V, 2V, 1V
  
  int ReadChannel(const uint8_t chan, double& value);
  int ReadEnabled(std::map<uint8_t, double>& values);
  int ReadAll(std::vector<double>& values);
};

#endif // __MCC_HAT_BOARD_128_H__
