/**
    Copyright (c) 2022 Darius Dauskurdis darius.dauskurdis@gmail.com
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

/**
    JSON data parser, which converts JSON string/file to map of objects
    @file JSONReader.h
    @author Darius Dauskurdis darius.dauskurdis@gmail.com
    @version 1.1 19/01/2022
*/

#ifndef JSONREADER_H
#define JSONREADER_H
#pragma once
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <string>

class JSONReader
{
	public:
		class JSONNode{
			public:
				std::string key;
				std::string value;
				std::string type;
				std::map<std::string, JSONNode*> childs;
				std::vector<std::string> childs_order;
		};
		/*std::map<std::string, JSONNode*> loadFromFile(std::string url);
		std::map<std::string, JSONNode*> load(std::string str);*/
		JSONNode *loadFromFile(std::string url);
		JSONNode *load(std::string str);
	private:
		std::fstream json_file;
		std::string json_text;
		int array_ref_index = 0;
		int object_ref_index = 0;
		int json_from_file;
		int current_character_index;
		char current_character;
		void readJSONText();
		bool isNumber(const std::string& str);
		bool isFloat(std::string str);
		JSONNode *parseObjectOrArray();
		JSONNode *parseJSONString(std::string type, std::string str);
		JSONNode *parseJSONObjectAndValue(std::string str);
		std::map<std::string, JSONNode*> temp_data;
};
 
#endif