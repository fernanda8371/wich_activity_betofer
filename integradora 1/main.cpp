#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


// Los archivos de transmisión contienen caracteres de texto que representan el envío de datos de un dispositivo a otro.
// El archivo mcode.txt representan posibles códigos maliciosos (uno por renglón) que se puede encontrar dentro de una transmisión.
void z_function(const string &t, const string &m) {
	string s = m + "@" + t;
    int n = s.size();
	vector<int> z(n);
    int l = 0, r = 0;
    for(int i = 1; i < n; i++) {
        if(i < r) {
            z[i] = min(r - i, z[i - l]);
        }
        while(i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if(i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
		if(z[i] == m.size()){
			cout << boolalpha << true << ' ' << i - m.size() << endl;
			return;
		}
    }
	cout << boolalpha << false << endl;
}

const int64_t m = 1000000007;
const int64_t p = 67;
int char_value(char c){
	if(c == '\n') return 1 + 26 + 26 + 10;
	else if(islower(c)) return c - 'a' + 1;
	else if(isupper(c)) return c - 'A' + 1 + 26;
	else if(isdigit(c)) return c - '0' + 1 + 26 + 26;
	else return 0;
}

vector<int64_t> pows;
vector<int64_t> compute_hash(string const& s) {
    int64_t hash_value = 0;
    int64_t p_pow = 1;
	pows.clear();
	vector<int64_t> prefix_hash;
    for (char c : s) {
        hash_value = (hash_value + char_value(c) * p_pow) % m;
		prefix_hash.push_back(hash_value);
		pows.push_back(p_pow);
        p_pow = (p_pow * p) % m;
    }
	pows.push_back(p_pow);
    return prefix_hash;
}

bool is_palindrome(int idx, int size, const vector<int64_t> &h, const vector<int64_t> &rh, bool even = false){
	int left = idx - size;
	int right = idx + size - even;

	if(left < 0 || right >= h.size()) return false;

	int64_t left_hash = h[right] - (left-1==0? 0: h[left-1]);
	int64_t right_hash = rh[left] - (right+1==h.size()? 0: rh[right+1]);

	int lts = left;
	int rte = (h.size()-1)-right;

	if (lts < rte)
		left_hash = (left_hash * pows[rte - lts]) % m;

	if (lts > rte)
		right_hash = (right_hash * pows[lts - rte]) % m;
		
	return left_hash == right_hash;
}

void longest_palindrome(const string &t, const vector<int64_t> &ht, const vector<int64_t> &hrt){
	int ai = 0, al = 1;

	for(int i = 0; i < t.size(); i++){
		int l = 0;
		int r = t.size();
		while(l <= r){
			int m = (r + l) / 2;
			if(is_palindrome(i, m, ht, hrt, 0)) {
				l = m + 1;
			} else {
				r = m - 1;
			}
		}

		l--;
		if(2 * l + 1 > al){
			ai = i - l;
			al = 2 * l + 1;
		}

		l = 0;
		r = t.size();
		while(l <= r){
			int m = (r + l) / 2;
			if(is_palindrome(i, m, ht, hrt, 1)) {
				l = m + 1;
			} else {
				r = m - 1;
			}
		}

		l--;
		if(2 * l > al){
			ai = i - l;
			al = 2 * l;
		}
	}

	cout << ai+1 << ' ' << ai + al << ' ';
	cout << t.substr(ai, al) << endl;
}

int main(){

	ifstream it1, it2,it3, mcode;
	it1.open("transmission1.txt");
	it2.open("transmission2.txt");
	it3.open("transmission3.txt");

	mcode.open("mcode.txt");

	string line;
	string t1, t2, t3, m1;
	while(getline(it1, line))
		t1 += line + '\n';
	while(getline(it2, line))
		t2 += line + '\n';
	while(getline(it3, line))
		t3 += line + '\n';
	while(getline(mcode, line))
		m1 += line + '\n';

	// parte uno: Z function
	cout << t1 << endl;
	cout << t2 << endl;
	z_function(t1, m1);
	z_function(t2, m1);
	z_function(t3,m1);
/// hasta aqui le movi (fer)


	// parte 2, string hashing
	
	// every char has a diferent value (up to 63)
	// smallest prime bigger than 63 is 67
	/* for(char c = 'a'; c <= 'z'; c++) cout << char_value(c) << ' '; */
	/* for(char c = 'A'; c <= 'Z'; c++) cout << char_value(c) << ' '; */
	/* for(char c = '0'; c <= '9'; c++) cout << char_value(c) << ' '; */
	/* cout << char_value('\n') << ' '; */
	/* cout << endl; */

	string rt1 = t1;
	reverse(rt1.begin(), rt1.end());
	vector<int64_t> ht1 = compute_hash(t1);
	vector<int64_t> hrt1 = compute_hash(rt1);
	reverse(hrt1.begin(), hrt1.end());
	longest_palindrome(t1, ht1, hrt1);

	string rt2 = t2;
	reverse(rt2.begin(), rt2.end());
	vector<int64_t> ht2 = compute_hash(t2);
	vector<int64_t> hrt2 = compute_hash(rt2);
	reverse(hrt2.begin(), hrt2.end());
	longest_palindrome(t2, ht2, hrt2);




	

	return 0;
}
