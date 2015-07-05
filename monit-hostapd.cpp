#include <map>
#include <iostream>
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

   string assoc("associated");
   string disassoc("disassociated");
   string connected("AP-STA-CONNECTED");

   smatch m,t;
   string line;

   while(getline(cin,line)) {
      if ( regex_search( line, m, mac_reg ) ) {
         if ( line.find(connected) != string::npos ) {
            add_list.clear();
         }
	 else if ( line.find(assoc) != string::npos ) {
            add_list[m.str()] = time(NULL);
            //cout << "associated at " << t.str() <<  ": " << m.str() << endl;
         }
         else if ( line.find(disassoc) != string::npos ) {
            map_t::const_iterator it = add_list.find( m.str() );
            if ( it != add_list.end() ) {
               time_t curtime = time(NULL);
               double dt = difftime( curtime, it->second );
               if ( dt < dtmax ) {
                  system("/etc/sbin/reset-wlan0.sh");
                  add_list.clear();
               }
               else
                  add_list.erase( it );
            }
            //cout << "disassociated: " << m.str() << endl;
         }
         else {
            //cout << "nothing to do: " << line << endl;
         }
      }
   }  // while loop
}
