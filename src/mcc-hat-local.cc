/** @file
 * @brief User command to access the E906 data catalog.
 *
 * Execute `e906-data-catalog help` to check the usage.
 */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <getopt.h>
#include <unistd.h>
#include "MccHatServer.h"
using namespace std;
void PrintHelp(std::ostream& os=std::cout);

//
// Main
//
int main(int argc, char** argv)
{
  int verb = 0;

  int opt;
  while ((opt = getopt(argc, argv, "vh")) != -1) {
    switch (opt) {
    case 'v':  verb++;  break;
    case 'h':  PrintHelp();  return 0;  break;
    default:
      cerr << "Invalid option.  Abort.\n";
      exit(1);
    }
  }
  //cout << "optind = " << optind << endl;
  
  //int n_par = argc - optind;
  //AssureNumPar(n_par, 1, "a command", "The 'help' command shows you all available commands.");
  //string comm = argv[optind];
  //if (comm == "help" || comm == "-h" || comm == "--help") {
  //  PrintHelp();
  //  return 0;
  //}

  MccHatServer* svr = MccHatServer::instance();
  svr->Verbosity(verb);

  svr->EnableBoard(0);
  auto b0 = svr->GetBoard(0);
  b0->Open();
  b0->EnableChannel(0, "T");
  b0->EnableChannel(3, "T");

  for (int ii = 0; ii < 10; ii++) {
    std::vector<double> values;
    b0->ReadAll(values);
    cout << ii;
    for (auto it = values.begin(); it != values.end(); it++) {
      cout << "\t" << *it;
    }
    cout << endl;
    sleep(1);
  }

  b0->Close();
  svr->DisableBoard(0);
  
  return 0;
}

//
// Functions
//
void PrintHelp(std::ostream& os)
{
  os << "e906-data-catalog\n"
     << "Program to access the E906 data catalog.\n"
     << "\n"
     << "  e906-data-catalog help\n"
     << "  e906-data-catalog ds-list\n"
     << "  e906-data-catalog ds-exist (dataset_name)\n"
     << endl;
}
