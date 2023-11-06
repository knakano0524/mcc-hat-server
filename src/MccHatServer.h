#ifndef __MCC_HAT_SERVER_H__
#define __MCC_HAT_SERVER_H__
#include <vector>
#include <map>
#include <string>
#include "MccHatBoard.h"

/// Singleton class to manage all loaded boards.
/**
 * To be written.
 */
class MccHatServer {
  static const int N_BOARD = 8;
  static MccHatServer* m_instance;
  MccHatServer();

  int m_verb;
  uint8_t m_hat_num;
  struct HatInfo* m_hat_info_list;
  MccHatBoard* m_list_board[N_BOARD];

 public:
  static MccHatServer* instance();
  virtual ~MccHatServer();

  int GetNumBoard() const { return N_BOARD; }
  
  void Verbosity(const int verb); ///< Set the verbosity.  0 = silent, 1 = info, 2 = debug, 3 = insane
  int  Verbosity() const  { return m_verb; } ///< Get the verbosity.

  bool IsValidBoard(const unsigned int board) { return 0 <= board && board < N_BOARD; }

  int InitBoards(const bool close_first=true);
  int CloseBoards();
  MccHatBoard* GetBoard(const unsigned int board);

  bool IsReady(const unsigned int board, std::ostream* os=0);
  
  int ExecCommand(const std::string command, std::string& output);
};

#endif // __MCC_HAT_SERVER_H__
