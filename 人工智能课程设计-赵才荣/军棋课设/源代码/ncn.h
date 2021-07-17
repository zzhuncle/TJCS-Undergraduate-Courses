#pragma once
#include<array>
#include<vector>
#include<iostream>
#include<unordered_map>
using namespace std;
void parse_ncn(array<array<int, 5>, 13>& board, string ncn, int& cnt, int& total_cnt);
string board_to_ncn(const array<array<int, 5>, 13>& board);
array<array<int, 2>, 2> parse_ncn_move(string move);
string move_to_ncn(const array<array<int, 2>, 2>& arr);
