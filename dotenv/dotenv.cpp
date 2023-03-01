/*
 * Copyright 2023 harshfeudal
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dotenv.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

 // Trim leading and trailing whitespace
std::string Trim(const std::string& str) {
	auto first = str.find_first_not_of(" \t\n\r");
	auto last = str.find_last_not_of(" \t\n\r");
	if (first == std::string::npos || last == std::string::npos) {
		return "";
	}
	return str.substr(first, last - first + 1);
}

// Check if a string is enclosed in double quotes
bool IsQuoted(const std::string & str) {
	return str.size() >= 2 && str.front() == '"' && str.back() == '"';
}

// Remove the enclosing double quotes from a string
std::string RemoveQuotes(const std::string & str) {
	if (IsQuoted(str)) {
		std::string result;
		bool escape = false;
		for (char c : str.substr(1, str.size() - 2)) {
			if (escape) {
				if (c == 'n') {
					result += '\n';
				}
				else if (c == 't') {
					result += '\t';
				}
				else {
					result += c;
				}
				escape = false;
			}
			else if (c == '\\') {
				escape = true;
			}
			else {
				result += c;
			}
		}
		return result;
	}
	return str;
}

Dotenv::Dotenv(const std::string & filename) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("failed to open file");
	}

	std::string line;
	while (std::getline(file, line)) {
		// Ignore comments and empty lines
		if (line.empty() || line[0] == '#' || line[0] == ';') {
			continue;
		}

		// Find the position of the first '=' character
		auto pos = line.find('=');
		if (pos == std::string::npos) {
			continue;
		}

		// Extract the key-value pair
		auto key = Trim(line.substr(0, pos));
		auto value = Trim(line.substr(pos + 1));

		// Remove quotes if present
		value = RemoveQuotes(value);

		// Store the key-value pair
		data_[key] = value;
	}
}

std::string Dotenv::Get(const std::string & key) const {
	auto it = data_.find(key);
	if (it != data_.end()) {
		return it->second;
	}
	return "";
}

bool Dotenv::Has(const std::string & key) const {
	return data_.find(key) != data_.end();
}