#include "NetworkCommon.h"

Vector3 InterpretStringVector(string message)
{
	Vector3 out;

	size_t xIdx = message.find_first_of(' ');
	size_t yIdx = message.substr(xIdx).find_first_of(' ');

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
	if (delim_idx) {
		chars.push_back(s.substr(0, delim_idx));
		vector<string> subs = split_string(s.substr(delim_idx), d);
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