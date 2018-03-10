/*               
                          .,okkkd:.                          
                       .:x0KOdooxKKkl,.                      
                   .,oOKKxc'. .. .;oOX0d:.                   
                ...oKOo;. .;dO00xc.  'cxKO, ..               
            .,lk0l...  .:oxXMMMMMWOoc'  .. ,O0d:.            
         .:d0XOo;.     ;c..kMMMMMK;.;:.     'ckKKkc'.        
      'lkKKxc'  .,.        oWMMMMO.        ''  .:d0KOo;.     
     '0Wk;. .,loo:.        :NMMMMx.        ,loo:. .,oXNc     
     ,0X: .lKWMKl.         ,KMMMWo         .;kWWXx' .kNc     
     '0X; .OMMMMWXx;.      ,0MMMNl       'o0NMMMMN: .kWc     
     '0X; .k0d0NWMMW0o,..cxKWMMMMXkl,..ckNMMMWKxkK: .kWc     
     '0X; .kl  ':okKWMNKXWMMMMMMMMMMNKXWWXOdc,. ,O: .kWc     
     '0X;  ,.      .,oXMMMMMMMMMMMMMMMWk;.      .;. .kNc     
     .,;.            '0MMMMMMMMMMMMMMMWc             ';.			Alexander Falk
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			NetworkCommon.cpp
     '0X: .:,     .,lkNMMMMMMMMMMMMMMMWKo:'.    .c' .OWl     
     '0X; .ko.':okXWMW0xkXWMMMMMMMMN0xkNMWN0xl;.:O: .OWc     
     '0X; .OX0NMMMWKx;.  .:xNMMMW0l,.  'lONMMMWKKX: .kWc     
     '0X: .OMMMMNkc.       '0MMMNc       .;dKWMMMN: .kWc     
     '0N: .;xKWKc.         ;XMMMWo          'kNXkl. .OWc     
     .xNKd:. .;loc.        cNMMMMk.       .;ol;. .,lONK;     
      .'lkKKkl,. .         dWMMWM0'        .  .:d0XOo;.      
          .:d0X0d,     ,l:;OMMMMMXl;lc.    .ckKKkc'          
             .,lxc.,c'. .:d0WMMMMXkl,. .;:.'dd:.             
                  .l0XOo;. .;oooc' .'cxKKx'                  
                    .,lkKKxc'.  .;oOK0d:.                    
                        .:d0K000KKkl,.                       
                           .,cll:.                            
*/
// Common Functions For the Network

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
	else
	{
		chars.push_back(s);
	}

	return chars;
}


PacketType FindType(string data)
{
	int i = stoi(data.substr(0, data.find_first_of(':')));

	return PacketType(i);
}

Vector3 LerpVector3(Vector3 a, Vector3 b, float f)
{
	Vector3 na = a * f;
	Vector3 nb = b * (1 - f);
	Vector3 c = na + nb;
	return c;
}