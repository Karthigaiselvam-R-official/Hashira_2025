Hashira Placements Assignment - Shamir Secret Sharing
This project implements Shamir's Secret Sharing scheme to reconstruct a secret from shares provided in JSON format. The solution handles shares in different bases, detects corrupted shares, and reconstructs the original secret using Lagrange interpolation.

Features
BigInt Class: Handles arbitrary-precision arithmetic for large numbers

Base Conversion: Converts numbers from any base (2-36) to decimal

Corruption Detection: Identifies corrupted shares using combinatorial verification

Secret Reconstruction: Uses Lagrange interpolation to reconstruct the secret from valid shares

Requirements
C++17 compatible compiler

JSONCPP library for JSON parsing

Installation
Install JSONCPP:

bash
sudo apt-get install libjsoncpp-dev
Compile the code:

bash
g++ -std=c++17 hashira.cpp -ljsoncpp -o hashira
Usage
Run the program with a JSON test case file:

bash
./hashira testcase1.json
JSON Input Format
The JSON file should contain:

n: Total number of shares

k: Minimum shares needed for reconstruction

Shares with keys as integers and values containing base and value

Example:

json
{
  "keys": {
    "n": 4,
    "k": 3
  },
  "1": {
    "base": "10",
    "value": "4"
  },
  "2": {
    "base": "2",
    "value": "111"
  }
}
Output
The program outputs:

Reconstructed secret

Indices of corrupted shares (0-based)

Test Cases
Test Case 1
Input:

json
{
  "keys": {
    "n": 4,
    "k": 3
  },
  "1": {
    "base": "10",
    "value": "4"
  },
  "2": {
    "base": "2",
    "value": "111"
  },
  "3": {
    "base": "10",
    "value": "12"
  },
  "6": {
    "base": "4",
    "value": "213"
  }
}
Output:

text
Secret: 3
Corrupted shares indices (0-based): 
Test Case 2
Input:

json
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

text
Secret: 1000000000000000000000000000000
Corrupted shares indices (0-based): 5 6
Answers
Output for TestCase - 1: 3

Output for TestCase - 2: 1000000000000000000000000000000

Wrong Data Set Points for Test Case-1: None (empty)

Wrong Data Set Points for Test Case-2: Indices 5 and 6 (0-based)

Implementation Details
BigInt Class: Handles large integers with base conversion capabilities

ShamirSecretSharing Class: Implements secret sharing operations

Corruption Detection: Uses combinatorial approach to identify inconsistent shares

Prime Field: Operations are performed modulo a large prime number

Error Handling
The program includes comprehensive error handling for:

Invalid JSON format

Missing required fields

Invalid number formats

Insufficient shares for reconstruction
