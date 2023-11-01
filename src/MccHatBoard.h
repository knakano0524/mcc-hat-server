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
  int EnableChannel (const uint8_t     chan);
  int EnableChannel (const std::string chan);
  int DisableChannel(const uint8_t     chan);
  int DisableChannel(const std::string chan);
    
  virtual int Open()  { return 99; }
  virtual int Close() { return 99; }
  virtual bool IsOpen() const { return false; }
  
  /// HAT 134
  virtual int GetTcType(const uint8_t chan, std::string& type) const { return 99; }
  virtual int SetTcType(const uint8_t chan, const std::string type) { return 99; }
  virtual int GetTcType(const std::string chan, std::string& type) const { return 99; }
  virtual int SetTcType(const std::string chan, const std::string type) { return 99; }

  /// HAT 128
  virtual int GetChannelOpts(const uint8_t chan, std::string& opts) const { return 99; }
  virtual int SetChannelOpts(const uint8_t chan, const std::string opts) { return 99; }
  virtual int GetChannelOpts(const std::string chan, std::string& opts) const { return 99; }
  virtual int SetChannelOpts(const std::string chan, const std::string opts) { return 99; }
  virtual int GetInputMode(std::string& mode) const { return 99;}
  virtual int SetInputMode(const std::string mode) { return 99;}
  virtual int GetInputRange(std::string& range) const { return 99;}
  virtual int SetInputRange(const std::string range) { return 99;}

  virtual int ReadChannel(const uint8_t chan, double& value) { return 99; }
  virtual int ReadEnabled(std::map<uint8_t, double>& values) { return 99; }
  virtual int ReadAll(std::vector<double>& values) { return 99; }

  int SplitChannelList(const std::string list_chan_str, std::vector<uint8_t>& list_chan) const;
};

#endif // __MCC_HAT_BOARD_H__
