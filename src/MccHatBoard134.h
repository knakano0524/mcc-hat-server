#ifndef __MCC_HAT_BOARD_134_H__
#define __MCC_HAT_BOARD_134_H__
#include "MccHatBoard.h"

/// Class to represent MCC HAT134.
class MccHatBoard134 : public MccHatBoard {
protected:
  static const uint8_t N_CHAN = 4;
  uint8_t m_tc_type[N_CHAN];
    
public:
  MccHatBoard134(const uint8_t addr);
  virtual ~MccHatBoard134();

  uint8_t TcType(const std::string type) const;
  std::string TcType(const uint8_t type) const;

  uint8_t GetNumChan() const { return N_CHAN; }

  int  Open()   { return mcc134_open (m_addr) == RESULT_SUCCESS; }
  int  Close()  { return mcc134_close(m_addr) == RESULT_SUCCESS; }
  bool IsOpen() { return mcc134_is_open(m_addr) == 1; }

  int EnableChannel(const uint8_t chan, const std::string opt="T");
  int EnableChannelListed(const std::string list_chan, const std::string opt="T");
  
  int EnableChannel(const uint8_t chan, const uint8_t tc_type);
  int EnableChannelListed(const std::string list_chan, const uint8_t tc_type);
  
  int DisableChannel(const uint8_t chan);

  int GetTcType(const uint8_t chan, std::string& type) const;
  int SetTcType(const uint8_t chan, const std::string type);
  int GetTcType(const std::string chan, std::string& type) const;
  int SetTcType(const std::string chan, const std::string type);
  
  int ReadChannel(const uint8_t chan, double& value);
  int ReadEnabled(std::map<uint8_t, double>& values);
  int ReadAll(std::vector<double>& values);
};

#endif // __MCC_HAT_BOARD_134_H__
