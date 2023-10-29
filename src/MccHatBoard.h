#ifndef __MCC_HAT_BOARD_H__
#define __MCC_HAT_BOARD_H__
#include <string>
#include <vector>
#include <map>
#include <daqhats/daqhats.h>
//#include <daqhats_utils>

/// Base class to represent one MCC HAT board.
class MccHatBoard {
protected:
  int m_verb; ///< 0 = silent, 1 = info, 2 = debug, 3 = insane
  uint8_t m_addr; // 0-7
  bool    m_enabled[8];

public:
  MccHatBoard(const uint8_t addr);
  virtual ~MccHatBoard();

  void Verbosity(const int verb) { m_verb = verb; } ///< Set the verbosity.
  int  Verbosity() const  { return m_verb; }        ///< Get the verbosity.

  uint8_t GetAddress() const { return m_addr; }
  
  virtual uint8_t GetNumChan() const { return 0; }
  bool IsValidChan(const uint8_t chan) const { return 0 <= chan && chan < GetNumChan(); }
  
  virtual int Open()  { return 1; }
  virtual int Close() { return 1; }
  virtual bool IsOpen() const { return false; }
  
  virtual int EnableChannel(const uint8_t chan, const std::string opt) { return 1; }
  virtual int EnableChannelListed(const std::string list_chan, const std::string opt) { return 1; }

  /// HAT 134
  virtual int EnableChannel(const uint8_t chan, const uint8_t tc_type) { return 1; }
  virtual int EnableChannelListed(const std::string list_chan, const uint8_t tc_type) { return 1; }

  /// HAT 128
  virtual int EnableChannel(const uint8_t chan, const uint32_t opt=0) { return 1; }
  virtual int EnableChannelListed(const std::string list_chan, const uint32_t opt=0) { return 1; }

  virtual int DisableChannel(const uint8_t chan) { return 1; }

  virtual int ReadChannel(const uint8_t chan, double& value) { return 1; }
  virtual int ReadEnabled(std::map<uint8_t, double>& values) { return 1; }
  virtual int ReadAll(std::vector<double>& values) { return 1; }

  int SplitChannelList(const std::string list_chan_str, std::vector<uint8_t>& list_chan);
};

#endif // __MCC_HAT_BOARD_H__
