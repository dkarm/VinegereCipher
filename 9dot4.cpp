#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void usage_instruction(string program_name)
{
	cout << "Usage: " << program_name << " [-d] -kn infile.txt outfile.txt" << endl;
	exit(1);
}

void open_file_error(string a)
{
	cout << "Cannot open " << a << endl;
	exit(1);
}


void create_key(vector<vector <int> >& keys_encrypt, string key_p, vector<vector <int> >& keys_decrypt, int i)
{
	int remaining_letters[26] = {0};
	int k = 0;
	int current = 0;
	if ('A' <= key_p[i] && key_p[i] <= 'Z')
	{
		current = key_p[i] - 'A';
	}
	if ('a' <= key_p[i] && key_p[i] <= 'z')	
	{
		current = key_p[i] - 'a' ;

	}

	keys_encrypt[i][k] = current-k;


	keys_decrypt[i][current]= k-current;
	k++;	
	
	while (k < 26)
	{
		current++;
		if (current>25)
		{
			current = 0;
		}

		keys_encrypt[i][k]=current-k;
		keys_decrypt[i][current]= k-current;
		k++;

	}
}

char encrypt(vector< int > keys, char ch)
{
		int current = 0;
		if ('A' <= ch && ch <= 'Z')
		{

			int current = ch - 'A';

			return static_cast<char>(ch + keys[current]); 
		}
		if ('a' <= ch && ch <= 'z')
		{
			int current = ch - 'a';

			return static_cast<char>(ch + keys[current]);
		}

	return ch;
}

void encrypt_file(ifstream& infile, ofstream& outfile, vector< vector<int> > keys, int key_length)
{
	char ch;
	int letter_count =0;
	while (infile.get(ch))
	{	

		int current_lookup = letter_count%key_length;

		outfile.put(encrypt(keys[current_lookup], ch));
		letter_count++;
	}
}

int main(int argc, char *argv[])
{
	bool decript = false;
	string key;
	bool key_specified = false;
	int n_file = 0;
	ifstream infile;
	ofstream outfile;
	
	if (argc < 4 || argc > 5) usage_instruction(string(argv[0]));

	for (int i = 1; i < argc; i++)
	{

		string arg = string(argv[i]);

		if (arg.length() >=2 && arg[0]=='-')
		{
			if(arg[1]=='d')
			{
				decript = true;
			}		
			if(arg[1]=='k')
			{

				key = arg.substr(2);
				key_specified = true;
			}
		}
		else
		{
			n_file++;
			if (n_file ==1)
			{
	
				infile.open(arg.c_str());	
				if( infile.fail()) 	open_file_error(arg);
			}
			if (n_file ==2)
			{

				outfile.open(arg.c_str());	
				if (outfile.fail()) open_file_error(arg); 
			}
		
		}

	}

	if (key_specified == false)
	{
		cout << "no key specified" << endl;
		return 1;
	}
	
	if (n_file !=2) usage_instruction(string(argv[0]));
	int key_length = key.length();
	int vecsize = 26;
		
	vector<vector<int> > array_of_key(key_length, std::vector<int>(26,0));
	vector<vector<int> > decrypt_key(key_length, std::vector<int>(26,0));
	
	for (int f = 0; f < key_length; f++)
	{
		create_key(array_of_key, key, decrypt_key, f);
	}
	
	if (decript) 
	{
	
		encrypt_file(infile, outfile, decrypt_key, key_length);
	}
	else
	{
		encrypt_file(infile, outfile, array_of_key, key_length);
	}
	
	infile.close();
	outfile.close();

	return 0;
}