#include <cstdlib>
#include <cstring>
#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <sstream>
#include "MccHatBoard128.h"
#include "MccHatBoard134.h"
#include "MccHatServer.h"
using namespace std;

MccHatServer* MccHatServer::m_instance = 0;

MccHatServer* MccHatServer::instance()
{
  if (! m_instance) m_instance = new MccHatServer();
  return m_instance;
}

MccHatServer::MccHatServer()
  : m_verb(0)
  , m_hat_num(0)
  , m_hat_info_list(0)
{
  memset(m_list_board, 0, sizeof(m_list_board));
}

MccHatServer::~MccHatServer()
{
  CloseBoards();
}

void MccHatServer::Verbosity(const int verb)
{
  m_verb = verb;
  for (int bd = 0; bd < N_BOARD; bd++) {
    if (m_list_board[bd]) m_list_board[bd]->Verbosity(verb);
  }
}

int MccHatServer::InitBoards(const bool close_first)
{
  cout << "MccHatServer::InitBoards()\n";
  if (close_first) CloseBoards();

  m_hat_num = hat_list(HAT_ID_ANY, NULL);
  m_hat_info_list = (struct HatInfo*)malloc(m_hat_num * sizeof(struct HatInfo));
  hat_list(HAT_ID_ANY, m_hat_info_list);

  for (int bd = 0; bd < m_hat_num; bd++) {
    uint8_t addr = m_hat_info_list[bd].address;
    uint16_t id  = m_hat_info_list[bd].id;
    uint16_t ver = m_hat_info_list[bd].version;
    string  name = m_hat_info_list[bd].product_name;
    switch (id) {
    case HAT_ID_MCC_118:
      cout << "Unsupported ID (" << id << ").  Skipped." << endl;
      //m_list_board[addr] = new MccHatBoard118(addr);
      break;
    case HAT_ID_MCC_128:
      m_list_board[addr] = new MccHatBoard128(addr);
      break;
    case HAT_ID_MCC_134:
      m_list_board[addr] = new MccHatBoard134(addr);
      break;
    case HAT_ID_MCC_152:
      cout << "Unsupported ID (" << id << ").  Skipped." << endl;
      //m_list_board[addr] = new MccHatBoard152(addr);
      break;
    case HAT_ID_MCC_172:
      cout << "Unsupported ID (" << id << ").  Skipped." << endl;
      //m_list_board[addr] = new MccHatBoard172(addr);
      break;
    default:
      cout << "Unknown ID (" << id << ").  Skipped." << endl;
      break;
    }
    if (m_list_board[addr]) {
      cout << "  " << bd << "\t" << addr << "\t" << id << "\t" << ver << "\t" << name << "\n";
      m_list_board[addr]->Open();
    }
  }    
  return 0;
}

int MccHatServer::CloseBoards()
{
  for (int bd = 0; bd < N_BOARD; bd++) {
    if (m_list_board[bd]) delete m_list_board[bd];
    m_list_board[bd] = 0;
  }
  return 0;
}

//int MccHatServer::EnableBoard(const unsigned int board)
//{
//  if (! IsValidBoard(board)) return 1;
//  if (m_list_board[board]) return 2;
//  m_list_board[board] = new MccHatBoard(board);
//  return 0;
//}
//
//int MccHatServer::DisableBoard(const unsigned int board)
//{
//  if (! IsValidBoard(board)) return 1;
//  if (m_list_board[board]) delete m_list_board[board];
//  return 0;
//}
//
//int MccHatServer::DisableAll()
//{
//  int ret = 0;
//  for (int bd = 0; bd < N_BOARD; bd++) ret += DisableBoard(bd);
//  return ret;
//}

MccHatBoard* MccHatServer::GetBoard(const unsigned int board)
{
  if (! IsValidBoard(board)) return 0;
  return m_list_board[board];
}

bool MccHatServer::IsReady(const unsigned int board, std::ostream* os)
{
  if (! IsValidBoard(board)) {
    if (os) *os << "Invalid board, " << board << "." << endl;
    return false;
  }
  if (! GetBoard(board)) {
    if (os) *os << "Board is not enabled." << endl;
    return false;
  }
  return true;
}

int MccHatServer::ExecCommand(const std::string command, std::string& output)
{
  FILE* fp = popen(command.c_str(), "r");
  if (fp == NULL) {
    cerr << "Failed at 'popen(daqhats_version)'." << endl;
    return 1;
  }
  char cmd_buffer[1024];
  output = "";
  while (fgets(cmd_buffer, sizeof(cmd_buffer), fp)) output += cmd_buffer;
  pclose(fp);
  return 0;
}
