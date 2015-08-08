#include <map>
#include <iostream>
#include <fstream>
#include <cstring>
#include <regex>
#include <ctime>


using namespace std;

const double dtmax = 8;

int main()
{
   typedef map<string,time_t> map_t;
   map_t add_list;

   regex mac_reg("[[:xdigit:]:]{17}");

   string assoc(" associated");
   string disassoc(" disassociated");
   string connected("AP-STA-CONNECTED");

   string logfile="/tmp/monit-hostapd.log";

   //ofstream of( logfile, ofstream::out | ofstream::app );
   //of << "Started logging\n";

   smatch m,t;
   string line;

   while(getline(cin,line)) {
      if ( regex_search( line, m, mac_reg ) ) {
         //of << line << endl;
         if ( line.find(connected) != string::npos ) {
            //of << m.str() << ": CONNECTED" << endl;
            add_list.clear();
         }
	      else if ( line.find(assoc) != string::npos ) {
            add_list[m.str()] = time(NULL);
            //of << m.str() << ": ASSOCIATED" << endl;
         }
         else if ( line.find(disassoc) != string::npos ) {
            //of << m.str() << ": DISASSOCIATED" << endl;
            map_t::const_iterator it = add_list.find( m.str() );
            if ( it != add_list.end() ) {
               time_t curtime = time(NULL);
               double dt = difftime( curtime, it->second );
               //of << "DELTA: " << dt << endl;
               if ( dt < dtmax ) {
                  //of << "MALFUNCION" << endl;
                  system("/git/sbin/reset-wlan0.sh");
                  add_list.clear();
               }
               else
                  add_list.erase( it );
            }
         }
      }
   }  // while loop
}
