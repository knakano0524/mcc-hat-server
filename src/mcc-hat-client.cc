/** @file
 * @brief Administrative command to manage the E906 data catalog.
 *
 * Execute `e906-data-catalog-admin help` to check the usage.
 */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
//#include <map>
//#include <getopt.h>
//#include "E906DataUploader.h"
using namespace std;
void AssureNumPar(const int n_par, const int n_par_req, const char* name, const char* msg=0);
void PrintHelp(std::ostream& os=std::cout);

///
/// Main
///
int main(int argc, char** argv)
{
  int n_par = argc - 1;
  if (n_par < 1) {
    cout << "Need a command.  Abort." << endl;
    exit(1);
  }
  string comm = argv[1];

  if (comm == "help" || comm == "-h" || comm == "--help") {
    PrintHelp();
  } else if (comm == "init-schema") {
    //E906DataUploader* du = E906DataUploader::instance();
    //du->InitSchema();
  } else if (comm == "upload-data") {
    //E906DataUploader* du = E906DataUploader::instance();
    //du->Upload("data", "dataset");
    //const vector<string> list_ds = { "ds57", "ds59", "ds62", "ds67", "ds70", "ds5a", "ds5b", "ds6" };
    //for (auto it = list_ds.begin(); it != list_ds.end(); it++) {
    //  du->Upload("data", *it + "/run");
    //  du->Upload("data", *it + "/spill");
    //  du->Upload("data", *it + "/pot_spill");
    //  du->Upload("data", *it + "/pot_total");
    //}
  } else {
    cout << "Command '" << comm << "' is not supported." << endl;
  }
  return 0;
}

//
// Functions
//
void AssureNumPar(const int n_par, const int n_par_req, const char* name, const char* msg)
{
  if (n_par < n_par_req) {
    cout << "Need " << name << " @ argument #" << n_par_req << ".  Abort." << endl;
    if (msg) cout << msg << endl;
    exit(1);
  }
}

void PrintHelp(std::ostream& os)
{
  os << "e906-data-catalog-admin\n"
     << "  Program to administrate the E906 data catalog.\n"
     << "\n"
     << "  e906-data-catalog-admin help\n"
     << "  e906-data-catalog-admin init-schema\n"
     << "  e906-data-catalog-admin upload-data\n"
     << endl;
}
