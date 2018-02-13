/*               
                          .,okkd:.                          
                       .:x0KOdoxKKkl,.                      
                   .,oOKKxc'. . .;oOX0d:.                   
                ...oKOo;. .;dO0xc.  'cxKO, ..               
            .,lk0l...  .:oxXMMMMWOoc'  .. ,O0d:.            
         .:d0XOo;.     ;c..kMMMMK;.;:.     'ckKKkc'.        
      'lkKKxc'  .,.        oWMMMO.        ''  .:d0KOo;.     
     '0Wk;. .,loo:.        :NMMMx.        ,loo:. .,oXNc     
     ,0X: .lKWMKl.         ,KMMWo         .;kWWXx' .kNc     
     '0X; .OMMMMWXx;.      ,0MMNl       'o0NMMMMN: .kWc     
     '0X; .k0d0NWMMW0o,..cxKWMMMXkl,..ckNMMMWKxkK: .kWc     
     '0X; .kl  ':okKWMNKXWMMMMMMMMMNKXWWXOdc,. ,O: .kWc     
     '0X;  ,.      .,oXMMMMMMMMMMMMMMWk;.      .;. .kNc     
     .,;.            '0MMMMMMMMMMMMMMWc             ';.			Alexander Falk
     .lo.            '0MMMMMMMMMMMMMMWc            .cd,			05/02/2018
     '0X: .:,     .,lkNMMMMMMMMMMMMMMWKo:'.    .c' .OWl     
     '0X; .ko.':okXWMW0xkXWMMMMMMMN0xkNMWN0xl;.:O: .OWc     
     '0X; .OX0NMMMWKx;.  .:xNMMW0l,.  'lONMMMWKKX: .kWc     
     '0X: .OMMMMNkc.       '0MMNc       .;dKWMMMN: .kWc     
     '0N: .;xKWKc.         ;XMMWo          'kNXkl. .OWc     
     .xNKd:. .;loc.        cNMMMk.       .;ol;. .,lONK;     
      .'lkKKkl,. .         dWMWM0'        .  .:d0XOo;.      
          .:d0X0d,     ,l:;OMMMMXl;lc.    .ckKKkc'          
             .,lxc.,c'. .:d0WMMMXkl,. .;:.'dd:.             
                  .l0XOo;. .;ooc' .'cxKKx'                  
                    .,lkKKxc'. .;oOK0d:.                    
                        .:d0K00KKkl,.                       
                           .,cl:.                            
*/

#include "NetworkCommon.h"

Vector3 InterpretStringVector(string message)
{
	Vector3 out;

	size_t xIdx = message.find_first_of(' ');
	size_t yIdx = xIdx + message.substr(xIdx + 1).find_first_of(' ');

	out.x = stof(message.substr(0, xIdx));
	out.y = stof(message.substr(xIdx + 1, yIdx));
	out.z = stof(message.substr(yIdx + 1));

	return out;
}

string Vector3ToString(Vector3 in)
{
	string out = to_string(in.x) + " " + to_string(in.y) + " " + to_string(in.z);
	return out;
}

vector<string> split_string(string s, char d) {
	vector<string> chars;

	size_t delim_idx = s.find_first_of(d);
	if (delim_idx != string::npos) {
		chars.push_back(s.substr(0, delim_idx));
		vector<string> subs = split_string(s.substr(delim_idx+1), d);
		for (vector<string>::iterator it = subs.begin(); it != subs.end(); ++it) {
			chars.push_back((*it));
		}
	}

	return chars;
}


PacketType FindType(string data)
{
	int i = stoi(data.substr(0, data.find_first_of(':')));

	return PacketType(i);
}

Player * Placeholder_GetPlayer(Colour id)
{
	return playerOne;
}