#include <vector>
#include <iostream>

const int m = 1e9 + 9;
const int p = 67;
int char_value(char c){
	if(c == '\n') return 1 + 26 + 26 + 10;
	else if(std::islower(c)) return c - 'a' + 1;
	else if(std::isupper(c)) return c - 'A' + 1 + 26;
	else if(std::isdigit(c)) return c - '0' + 1 + 26 + 26;
	else return 0;
}

std::vector<int64_t> pows;
std::vector<int64_t> compute_hash(std::string const& s) {
    int64_t hash_value = 0;
    int64_t p_pow = 1;
	pows.clear();
	std::vector<int64_t> prefix_hash;
    for (char c : s) {
        hash_value = (hash_value + char_value(c) * p_pow) % m;
		prefix_hash.push_back(hash_value);
		pows.push_back(p_pow);
        p_pow = (p_pow * p) % m;
    }
	pows.push_back(p_pow);
    return prefix_hash;
}

bool is_palindrome(int idx, int size, const std::vector<int64_t> &h, const std::vector<int64_t> &rh, bool even = false){
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

int main(){
	std::string word = "44dD242Dd77345";
	std::string rword = word;
	std::reverse(rword.begin(), rword.end());

	auto hash = compute_hash(word);
	auto rhash = compute_hash(rword);

	std::reverse(rhash.begin(), rhash.end());

	for(int64_t it: hash) std::cout << it << ' ';
	std::cout << std::endl;

	for(int64_t it: rhash) std::cout << it << ' ';
	std::cout << std::endl;

	std::cout << is_palindrome(5, 3, hash, rhash, 0) << std::endl;

	return 0;
}
