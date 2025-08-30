#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <json/json.h>

using namespace std;

class BigInt {
private:
    vector<int> digits;
    bool negative;
    static const int BASE = 1000000000;
    
public:
    BigInt(long long num = 0) : negative(num < 0) {
        num = abs(num);
        if (num == 0) digits.push_back(0);
        while (num > 0) {
            digits.push_back(num % BASE);
            num /= BASE;
        }
    }
    
    BigInt(const string& str, int base = 10) : negative(false) {
        if (str.empty()) {
            digits.push_back(0);
            return;
        }
        
        int start = 0;
        if (str[0] == '-') {
            negative = true;
            start = 1;
        }
        
        BigInt result(0);
        BigInt base_power(1);
        
        for (int i = str.length() - 1; i >= start; i--) {
            int digit_val;
            if (str[i] >= '0' && str[i] <= '9') {
                digit_val = str[i] - '0';
            } else if (str[i] >= 'A' && str[i] <= 'Z') {
                digit_val = str[i] - 'A' + 10;
            } else if (str[i] >= 'a' && str[i] <= 'z') {
                digit_val = str[i] - 'a' + 10;
            } else {
                throw invalid_argument("Invalid character in number string");
            }
            
            result = result + (base_power * BigInt(digit_val));
            base_power = base_power * BigInt(base);
        }
        
        *this = result;
    }
    
    BigInt operator+(const BigInt& other) const {
        if (negative != other.negative) {
            if (negative) return other - (-*this);
            return *this - (-other);
        }
        
        BigInt result;
        result.negative = negative;
        result.digits.clear();
        
        int carry = 0;
        size_t max_size = max(digits.size(), other.digits.size());
        
        for (size_t i = 0; i < max_size || carry; i++) {
            long long sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < other.digits.size()) sum += other.digits[i];
            
            result.digits.push_back(sum % BASE);
            carry = sum / BASE;
        }
        
        return result;
    }
    
    BigInt operator-(const BigInt& other) const {
        if (negative != other.negative) {
            return *this + (-other);
        }
        
        if (negative) return -((-other) - (-*this));
        
        if (*this < other) {
            BigInt result = other - *this;
            result.negative = true;
            return result;
        }
        
        BigInt result;
        result.digits.clear();
        
        int borrow = 0;
        for (size_t i = 0; i < digits.size(); i++) {
            long long diff = digits[i] - borrow;
            if (i < other.digits.size()) diff -= other.digits[i];
            
            if (diff < 0) {
                diff += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            
            result.digits.push_back(diff);
        }
        
        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }
        
        return result;
    }
    
    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result.digits.assign(digits.size() + other.digits.size(), 0);
        result.negative = negative != other.negative;
        
        for (size_t i = 0; i < digits.size(); i++) {
            for (size_t j = 0; j < other.digits.size(); j++) {
                long long prod = (long long)digits[i] * other.digits[j];
                result.digits[i + j] += prod % BASE;
                result.digits[i + j + 1] += prod / BASE;
                
                if (result.digits[i + j] >= BASE) {
                    result.digits[i + j + 1] += result.digits[i + j] / BASE;
                    result.digits[i + j] %= BASE;
                }
            }
        }
        
        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }
        
        return result;
    }
    
    BigInt operator%(const BigInt& mod) const {
        BigInt result = *this;
        while (result >= mod) {
            result = result - mod;
        }
        return result;
    }
    
    bool operator<(const BigInt& other) const {
        if (negative != other.negative) return negative;
        
        if (digits.size() != other.digits.size()) {
            return negative ? digits.size() > other.digits.size() : 
                             digits.size() < other.digits.size();
        }
        
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] != other.digits[i]) {
                return negative ? digits[i] > other.digits[i] : 
                                 digits[i] < other.digits[i];
            }
        }
        return false;
    }
    
    bool operator==(const BigInt& other) const {
        return negative == other.negative && digits == other.digits;
    }
    
    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }
    
    BigInt operator-() const {
        BigInt result = *this;
        result.negative = !negative;
        return result;
    }
    
    string toString() const {
        if (digits.empty()) return "0";
        
        stringstream ss;
        if (negative) ss << "-";
        
        ss << digits.back();
        for (int i = digits.size() - 2; i >= 0; i--) {
            ss << setfill('0') << setw(9) << digits[i];
        }
        
        return ss.str();
    }
};

class ShamirSecretSharing {
private:
    BigInt prime;
    int threshold;
    int num_shares;
    mt19937 rng;
    
public:
    struct Share {
        int x;
        BigInt y;
        Share(int x_val, const BigInt& y_val) : x(x_val), y(y_val) {}
    };
    
    ShamirSecretSharing(int k, int n, const BigInt& field_prime) 
        : threshold(k), num_shares(n), prime(field_prime), rng(random_device{}()) {
        if (k > n) throw invalid_argument("Threshold cannot exceed number of shares");
    }
    
    vector<BigInt> generatePolynomial(const BigInt& secret) {
        vector<BigInt> coeffs(threshold);
        coeffs[0] = secret;
        
        for (int i = 1; i < threshold; i++) {
            long long rand_val = uniform_int_distribution<long long>(1, 999999999)(rng);
            coeffs[i] = BigInt(rand_val) % prime;
        }
        
        return coeffs;
    }
    
    BigInt evaluatePolynomial(const vector<BigInt>& coeffs, int x) {
        BigInt result(0);
        BigInt x_power(1);
        
        for (const auto& coeff : coeffs) {
            result = (result + (coeff * x_power)) % prime;
            x_power = (x_power * BigInt(x)) % prime;
        }
        
        return result;
    }
    
    vector<Share> generateShares(const BigInt& secret) {
        auto coeffs = generatePolynomial(secret);
        vector<Share> shares;
        
        for (int i = 1; i <= num_shares; i++) {
            BigInt y = evaluatePolynomial(coeffs, i);
            shares.emplace_back(i, y);
        }
        
        return shares;
    }
    
    BigInt lagrangeInterpolation(const vector<Share>& shares) {
        if (shares.size() < threshold) {
            throw invalid_argument("Insufficient shares for reconstruction");
        }
        
        BigInt secret(0);
        
        for (size_t i = 0; i < threshold; i++) {
            BigInt numerator(1);
            BigInt denominator(1);
            
            for (size_t j = 0; j < threshold; j++) {
                if (i != j) {
                    numerator = (numerator * BigInt(-shares[j].x)) % prime;
                    denominator = (denominator * BigInt(shares[i].x - shares[j].x)) % prime;
                }
            }
            
            BigInt term = (shares[i].y * numerator * modInverse(denominator)) % prime;
            secret = (secret + term) % prime;
        }
        
        return secret;
    }
    
    vector<int> detectCorruptedShares(const vector<Share>& shares) {
        vector<int> corrupted_indices;
        
        if (shares.size() <= threshold) {
            return corrupted_indices;
        }
        
        vector<bool> selector(shares.size());
        fill(selector.begin(), selector.begin() + threshold, true);
        
        map<string, int> secret_counts;
        
        do {
            vector<Share> subset;
            vector<int> indices;
            
            for (size_t i = 0; i < shares.size(); i++) {
                if (selector[i]) {
                    subset.push_back(shares[i]);
                    indices.push_back(i);
                }
            }
            
            try {
                BigInt reconstructed_secret = lagrangeInterpolation(subset);
                string secret_str = reconstructed_secret.toString();
                secret_counts[secret_str]++;
            } catch (...) {
                continue;
            }
            
        } while (prev_permutation(selector.begin(), selector.end()));
        
        string correct_secret;
        int max_count = 0;
        
        for (const auto& pair : secret_counts) {
            if (pair.second > max_count) {
                max_count = pair.second;
                correct_secret = pair.first;
            }
        }
        
        vector<bool> is_correct(shares.size(), false);
        fill(selector.begin(), selector.end(), false);
        fill(selector.begin(), selector.begin() + threshold, true);
        
        do {
            vector<Share> subset;
            vector<int> indices;
            
            for (size_t i = 0; i < shares.size(); i++) {
                if (selector[i]) {
                    subset.push_back(shares[i]);
                    indices.push_back(i);
                }
            }
            
            try {
                BigInt reconstructed = lagrangeInterpolation(subset);
                if (reconstructed.toString() == correct_secret) {
                    for (int idx : indices) {
                        is_correct[idx] = true;
                    }
                }
            } catch (...) {
                continue;
            }
            
        } while (prev_permutation(selector.begin(), selector.end()));
        
        for (size_t i = 0; i < shares.size(); i++) {
            if (!is_correct[i]) {
                corrupted_indices.push_back(i);
            }
        }
        
        return corrupted_indices;
    }
    
    BigInt modInverse(const BigInt& a) {
        for (long long i = 1; i < 1000000; i++) {
            if (((a * BigInt(i)) % prime) == BigInt(1)) {
                return BigInt(i);
            }
        }
        throw runtime_error("Modular inverse not found");
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <testcase.json>" << endl;
        return 1;
    }

    try {
        ifstream file(argv[1]);
        if (!file.is_open()) {
            throw runtime_error("Could not open file: " + string(argv[1]));
        }

        Json::Value root;
        file >> root;

        int n = root["keys"]["n"].asInt();
        int k = root["keys"]["k"].asInt();

        BigInt prime("1000000000000000000000000000057");

        vector<ShamirSecretSharing::Share> shares;

        for (const auto& key : root.getMemberNames()) {
            if (key == "keys") continue;
            int x = stoi(key);
            Json::Value shareData = root[key];
            int base = shareData["base"].asInt();
            string value_str = shareData["value"].asString();
            BigInt y(value_str, base);
            shares.push_back(ShamirSecretSharing::Share(x, y));
        }

        ShamirSecretSharing sss(k, n, prime);

        vector<int> corrupted = sss.detectCorruptedShares(shares);

        vector<ShamirSecretSharing::Share> good_shares;
        for (size_t i = 0; i < shares.size(); i++) {
            if (find(corrupted.begin(), corrupted.end(), i) == corrupted.end()) {
                good_shares.push_back(shares[i]);
            }
        }

        if (good_shares.size() < k) {
            cerr << "Not enough good shares to reconstruct the secret." << endl;
            return 1;
        }

        BigInt secret = sss.lagrangeInterpolation(good_shares);

        cout << "Secret: " << secret.toString() << endl;
        cout << "Corrupted shares indices (0-based): ";
        for (int idx : corrupted) {
            cout << idx << " ";
        }
        cout << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
