# 🔐 HASHIRA - Shamir Secret Sharing System

*A high-performance C++ implementation of cryptographic secret sharing with corruption detection*

---

## 🚀 Features

### 🧮 BigInt Arithmetic Engine
- **Arbitrary-precision mathematics** with base conversion (2-36)
- **Optimized memory management** using base-10⁹ digit storage
- **Full arithmetic operations** (+, -, *, %, comparisons)

### 🎯 Shamir's Secret Sharing
- **Polynomial generation** with random coefficients
- **Share distribution** across multiple parties
- **Lagrange interpolation** for secret reconstruction
- **Modular arithmetic** in large prime fields

### 🔍 Advanced Corruption Detection
- **Combinatorial verification** of share consistency
- **Majority voting system** to identify corrupted shares
- **Automatic recovery** using valid shares only

---

## ⚡ Quick Start

### Prerequisites
bash
# Install JSONCPP
sudo apt-get install libjsoncpp-dev

# Compile with C++17
g++ -std=c++17 hashira.cpp -ljsoncpp -o hashira -O3

Run the system:
# Execute with test cases
./hashira testcase1.json
./hashira testcase2.json

🧪 Test Results
✅ Test Case 1 - Basic Verification
Input: 4 shares, threshold 3

json
{"keys":{"n":4,"k":3},"1":{"base":"10","value":"4"},"2":{"base":"2","value":"111"},"3":{"base":"10","value":"12"},"6":{"base":"4","value":"213"}}

Output:

text
🎯 Secret: 3
✅ All shares valid - No corruption detected

🛡️ Test Case 2 - Corruption Detection
Input: 10 shares, threshold 7 (with 2 corrupted)
{
"keys": {
    "n": 10,
    "k": 7
  },
  "1": {
    "base": "6",
    "value": "13444211440455345511"
  },
  "2": {
    "base": "15",
    "value": "aed7015a346d635"
  },
  "3": {
    "base": "15",
    "value": "6aeeb69631c227c"
  },
  "4": {
    "base": "16",
    "value": "e1b5e05623d881f"
  },
  "5": {
    "base": "8",
    "value": "316034514573652620673"
  },
  "6": {
    "base": "3",
    "value": "2122212201122002221120200210011020220200"
  },
  "7": {
    "base": "3",
    "value": "20120221122211000100210021102001201112121"
  },
  "8": {
    "base": "6",
    "value": "20220554335330240002224253"
  },
  "9": {
    "base": "12",
    "value": "45153788322a1255483"
  },
  "10": {
    "base": "7",
    "value": "1101613130313526312514143"
  }
}


Output:
🎯 Secret: 1000000000000000000000000000000
🚨 Corrupted shares detected at indices: 5, 6

📊 Performance Metrics

Base conversion: O(n) complexity per share

Interpolation: O(k²) for k shares

Corruption detection: O(C(n,k)) combinations

Memory efficient: 9-digit packing in vectors


🔧 Error Handling

The system includes robust error handling for:

❌ Invalid JSON syntax and missing fields

❌ Malformed number strings and invalid bases

❌ Insufficient shares for reconstruction

❌ Modular inverse computation failures



🏆 Answers Summary
Test Case	               Secret                  	Corrupted Shares
#1	                       3	                          None
#2	      1000000000000000000000000000000	          5, 6 (0-based)

