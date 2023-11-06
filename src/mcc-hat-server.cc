/** @file
 * @brief Main executable to launch the MccHat server.
 */
#include <cstdlib>
#include <iostream>
//#include <fstream>
#include <sstream>
//#include <cstring>
#include <vector>
#include <getopt.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "MccHatServer.h"
using namespace std;
int SendText(int fd, std::string line);
int SendStatus(const int fd, const int status, const std::string msg="");
int ReadLine(int fd, std::string& line, std::string& extra, const int verb=0);
int SplitLine(std::string line, std::vector<std::string>& words);
void PrintHelp(std::ostream& os=std::cout);

//
// Main
//
int main(int argc, char** argv)
{
  int port = 1111;
  //vector<string> list_client; // white list
  bool read_eeproms = false;
  bool init_board = false;
  int verb = 0;

  int opt;
  while ((opt = getopt(argc, argv, "p:eivh")) != -1) {
    switch (opt) {
    case 'p':  port = atoi(optarg);  break;
    case 'e':  read_eeproms = true;  break;
    case 'i':  init_board = true;  break;
    case 'v':  verb++;  break;
    case 'h':  PrintHelp();  return 0;  break;
    default:
      cerr << "Invalid option.  Abort.\n";
      exit(1);
    }
  }
  //cout << "optind = " << optind << endl;
  if (verb > 0) {
    cout << "Server configuration:\n"
	 << "  Port         = " << port << "\n"
	 << "  Read EEPROMs = " << read_eeproms << "\n"
	 << "  Init board   = " << init_board << "\n"
	 << "  Verbosity    = " << verb << endl;
  }
  //int n_par = argc - optind;

  MccHatServer* mhs = MccHatServer::instance();
  //mhs->Verbosity(verb);
  if (read_eeproms) {
    string output;
    mhs->ExecCommand("sudo daqhats_read_eeproms", output);
    cout << output;
  }
  if (init_board) mhs->InitBoards();

  if (verb > 0) cout << "Set up the socket." << endl;
  int fd_svr = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_svr < 0) {
    cerr << "Failed at 'socket()'.  Abort." << endl;
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr_svr;
  int sock_opt = 1;
  if (setsockopt(fd_svr, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &sock_opt, sizeof(sock_opt))) {
    cerr << "Failed at 'setsockopt()'.  Abort." << endl;
    exit(EXIT_FAILURE);
  }
  addr_svr.sin_family      = AF_INET;
  addr_svr.sin_addr.s_addr = INADDR_ANY;
  addr_svr.sin_port        = htons(port);

  if (bind(fd_svr, (struct sockaddr*) &addr_svr, sizeof(addr_svr)) < 0) {
    cerr << "Failed at 'bind()'.  Abort." << endl;
    exit(EXIT_FAILURE);
  }

  listen(fd_svr, 5);

  struct sockaddr_in addr_cli;
  socklen_t len_cli = sizeof(addr_cli);
  while (true) {
    if (verb > 0) cout << "Accepting a connection..." << endl;
    int fd_cli = accept(fd_svr, (struct sockaddr*) &addr_cli, &len_cli);
    if (fd_cli < 0) {
      cerr << "  Failed at 'accept()'.  Continue." << endl;
      continue;
    }

    char hname[99];
    gethostname(hname, 99);
    SendText(fd_cli, (string)hname + "\n");
    
    string line, extra = "";
    while (true) {
      SendText(fd_cli, "MccHat>"); // Prompt
      if (verb > 1) cout << "Reading a line..." << endl;
      int ret = ReadLine(fd_cli, line, extra, verb);
      if (ret != 0) {
	cerr << "  Failed at ReadLine(fd_cli).  Disconnect." << endl;
	break;
      }
      if (verb > 1) cout << "  Line = '" << line << "'." << endl;

      vector<string> words;
      SplitLine(line, words);
      if (words.size() == 0) {
	if (verb > 2) cout << "No command.  Continue." << endl;
      } else if (words[0] == "quit" || words[0] == "q") {
	SendStatus(fd_cli, 0);
	break;
      } else if (words[0] == "help" || words[0] == "h") {
	SendText(fd_cli, "Global Commands:\n");
	SendText(fd_cli, "  quit\n");
	SendText(fd_cli, "  daqhats-version\n");
	SendText(fd_cli, "  daqhats-list-boards\n");
	SendText(fd_cli, "  daqhats-read-eeproms\n");
	SendText(fd_cli, "  init-boards\n");
	SendText(fd_cli, "  enable-channel (board) (channel_list)\n");
	SendText(fd_cli, "  disable-channel (board) (channel_list)\n");
	SendText(fd_cli, "  read [all]\n");
	SendText(fd_cli, "\n");
	SendText(fd_cli, "  Arguments:\n");
	SendText(fd_cli, "    (board)       : 0, 1,,, 7\n");
	SendText(fd_cli, "    (channel_list): all, 0, 1, 0-2, 1,3-6, etc\n");
	SendText(fd_cli, "\n");
	SendText(fd_cli, "Commands for HAT 118:\n");
	SendText(fd_cli, "  get-channel-opts (board) (channel_list)\n");
	SendText(fd_cli, "  set-channel-opts (board) (channel_list) (noscale|nocalib)\n");
	SendText(fd_cli, "\n");
	SendText(fd_cli, "Commands for HAT 128:\n");
	SendText(fd_cli, "  get-channel-opts (board) (channel_list)\n");
	SendText(fd_cli, "  set-channel-opts (board) (channel_list) (noscale|nocalib)\n");
	SendText(fd_cli, "  get-input-mode (board)\n");
	SendText(fd_cli, "  set-input-mode (board) (SE|DIFF)\n");
	SendText(fd_cli, "  get-input-range (board)\n");
	SendText(fd_cli, "  set-input-range (board) (10V|5V|2V|1V)\n");
	SendText(fd_cli, "\n");
	SendText(fd_cli, "Commands for HAT 134:\n");
	SendText(fd_cli, "  get-tc-type (board) (channel_list)\n");
	SendText(fd_cli, "  set-tc-type (board) (channel_list) (type)\n");
      } else if (words[0] == "daqhats-version" || words[0] == "d-v") {
	string output;
	mhs->ExecCommand("daqhats_version", output);	
	SendText(fd_cli, output+"\n");
      } else if (words[0] == "daqhats-list-boards" || words[0] == "d-l-b") {
	string output;
	mhs->ExecCommand("daqhats_list_boards", output);	
	SendText(fd_cli, output+"\n");
      } else if (words[0] == "daqhats_read_eeproms" || words[0] == "d-r-e") {
	string output;
	mhs->ExecCommand("sudo daqhats_read_eeproms", output);	
	SendText(fd_cli, output+"\n");
      } else if (words[0] == "init-boards" || words[0] == "i-b") {
	SendStatus(fd_cli, mhs->InitBoards());
      } else if (words[0] == "enable-channel" || words[0] == "e-c") {
	int   board = atoi(words[1].c_str());
	string chan =      words[2]         ;
	if (! mhs->IsReady(board, &cout)) continue;
	SendStatus(fd_cli, mhs->GetBoard(board)->EnableChannel(chan));
      } else if (words[0] == "disable-channel" || words[0] == "d-c") {
	int   board = atoi(words[1].c_str());
	string chan =      words[2]         ;
	if (! mhs->IsReady(board, &cout)) continue;
	SendStatus(fd_cli, mhs->GetBoard(board)->DisableChannel(chan));
      } else if (words[0] == "get-channel-opts" || words[0] == "g-c-o") {
	int   board = atoi(words[1].c_str());
	string chan =      words[2].c_str() ;	
	string mode;
	if (! mhs->IsReady(board, &cout)) mode = "NG";
	else mhs->GetBoard(board)->GetChannelOpts(chan, mode);
	SendText(fd_cli, mode+"\n");
      } else if (words[0] == "set-channel-opts" || words[0] == "s-c-o") {
	int   board = atoi(words[1].c_str());
	string chan =      words[2].c_str() ;	
	string mode =      words[3].c_str() ;
	if (! mhs->IsReady(board, &cout)) continue;
	SendStatus(fd_cli, mhs->GetBoard(board)->SetChannelOpts(chan, mode));
      } else if (words[0] == "get-input-mode" || words[0] == "g-i-m") {
	int board = atoi(words[1].c_str());
	string mode;
	if (! mhs->IsReady(board, &cout)) mode = "NG";
	else mhs->GetBoard(board)->GetInputMode(mode);
	SendText(fd_cli, mode+"\n");
      } else if (words[0] == "set-input-mode" || words[0] == "s-i-m") {
	int   board = atoi(words[1].c_str());
	string mode =      words[2].c_str() ;
	if (! mhs->IsReady(board, &cout)) continue;
	SendStatus(fd_cli, mhs->GetBoard(board)->SetInputMode(mode));
      } else if (words[0] == "get-input-range" || words[0] == "g-i-r") {
	int board = atoi(words[1].c_str());
	string range;
	if (! mhs->IsReady(board, &cout)) range = "NG";
	else mhs->GetBoard(board)->GetInputRange(range);
	SendText(fd_cli, range+"\n");
      } else if (words[0] == "set-input-range" || words[0] == "s-i-r") {
	int    board = atoi(words[1].c_str());
	string range =      words[2].c_str() ;
	if (! mhs->IsReady(board, &cout)) continue;
	SendStatus(fd_cli, mhs->GetBoard(board)->SetInputRange(range));
      } else if (words[0] == "get-tc-type" || words[0] == "g-t-t") {
	int   board = atoi(words[1].c_str());
	string chan =      words[2].c_str() ;
	string type;
	if (! mhs->IsReady(board, &cout)) type = "NG";
	else mhs->GetBoard(board)->GetTcType(chan, type);
	SendText(fd_cli, type+"\n");
      } else if (words[0] == "set-tc-type" || words[0] == "s-t-t") {
	int    board = atoi(words[1].c_str());
	string chan =       words[2].c_str() ;
	string type =       words[3].c_str() ;
	if (! mhs->IsReady(board, &cout)) continue;
	SendStatus(fd_cli, mhs->GetBoard(board)->SetTcType(chan, type));
      } else if (words[0] == "read" || words[0] == "r") {
	ostringstream oss;
	if (words.size() > 1 && (words[1] == "all" || words[1] == "a") ) {
	  for (int i_bd = 0; i_bd < mhs->GetNumBoard(); i_bd++) {
	    MccHatBoard* bd = mhs->GetBoard(i_bd);
	    if (! bd) continue;
	    uint8_t addr = bd->GetAddress();
	    oss << (int)addr;
	    std::vector<double> values;	
	    bd->ReadAll(values);
	    for (auto it = values.begin(); it != values.end(); it++) oss << "\t" << *it;
	    oss << "\n";
	  }
	} else { // map
	  for (int i_bd = 0; i_bd < mhs->GetNumBoard(); i_bd++) {
	    MccHatBoard* bd = mhs->GetBoard(i_bd);
	    if (! bd) continue;
	    uint8_t addr = bd->GetAddress();
	    std::map<uint8_t, double> values;	
	    bd->ReadEnabled(values);
	    for (auto it = values.begin(); it != values.end(); it++) {
	      oss << (int)addr << "\t" << (int)it->first << "\t" << it->second << "\n";
	    }
	  }
	}
	SendText(fd_cli, oss.str());
      } else {
	SendStatus(fd_cli, 1, "Unknown command.");
	cout << "Unknown command: '" << line << "''  Ignore." << endl;
      }
    }
    
    close(fd_cli);
  } 
  close(fd_svr);
  return 0;
}

//
// Functions
//
int SendText(int fd, std::string text)
{
  return send(fd, text.c_str(), text.length(), 0);
}

int SendStatus(const int fd, const int status, const std::string msg)
{
  string text;
  if (status == 0) text = "OK\n";
  else {
    ostringstream oss;
    oss << "NG\t" << status;
    if (msg != "") oss << "\t" << msg;
    oss << "\n";
    text = oss.str();
  }
  return send(fd, text.c_str(), text.length(), 0);
}

int ReadLine(int fd, std::string& line, std::string& extra, const int verb)
{
  line = "";
  if (verb > 3) cout << "  ReadLine(): extra = '" << extra << "'." << endl;
  string buffer_str = extra;
  size_t pos;
  while ((pos = buffer_str.find("\n")) == string::npos) {
    line += buffer_str;
    char buffer_char[1024];
    ssize_t num = read(fd, buffer_char, 1022);
    if (num <= 0) {
      cerr << "ReadLine() failed (" << num << ").  Disconnect." << endl;
      return 1;
    }
    buffer_char[num] = '\0';
    buffer_str = buffer_char;
  }
  line += buffer_str.substr(0, pos);
  if (verb > 3) cout << "  length = " << buffer_str.length() << ", pos = " << pos << "." << endl;
  if (buffer_str.length() > pos + 1) extra = buffer_str.substr(pos + 1);
  else                               extra = "";
  return 0;
}

int SplitLine(std::string line, std::vector<std::string>& words)
{
  words.clear();
  istringstream iss(line);
  string word;
  while (iss >> word) words.push_back(word);
  return 0;
}

void PrintHelp(std::ostream& os)
{
  cout << "mcc-hat-server\n"
       << "Program to accept commands over TCP/IP to control MCC HAT.\n"
       << "\n"
       << "Options:"
       << "  -p (port) --- Change the port (default = 1111).\n"
       << "  -e        --- Read EEPROMs during startup.\n"
       << "  -i        --- Initalize all boards during startup.\n"
       << "  -v        --- Increase the verbosity by one.\n"
       << "  -h        --- Print this help message.\n"
       << endl;
}
