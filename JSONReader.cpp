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
    JSON data parser, which converts JSON string/file to unordered_map of JSONNode objects
    @file JSONReader.cpp
    @author Darius Dauskurdis darius.dauskurdis@gmail.com
    @version 1.1 19/01/2022
*/

#include "JSONReader.h"


/**
 * Read JSON data from file.
 *
 * @param url - address of JSON file.
 * @return map of JSONNode objects.
 */

JSONReader :: JSONNode* JSONReader:: loadFromFile(std::string url){
	json_file.open(url); // Open file from url
	JSONNode *json_node = new JSONNode; // Created JSONNode class object
  	if (json_file.is_open()){ // If JSON file opened successfully, start reading
  		json_from_file = 1; // Notice that data will be read from file
		json_node = JSONReader :: parseObjectOrArray(); // Call parsing method 
		json_file.close(); // Close file after reading
  	}
	return json_node;
}	


/**
 * Read JSON data from string.
 *
 * @param str - string of JSON data.
 * @return map of JSONNode objects.
 */

JSONReader :: JSONNode* JSONReader :: load(std::string str){
	json_from_file = 0; // Notice that data will be read from string and not from file
	current_character_index = -1; // Added default index value before reading JSON string
	json_text = str; // assigning JSON string to global variable
	JSONNode *json_node = new JSONNode; // Created JSONNode class object
	json_node = JSONReader :: parseObjectOrArray(); // Call parsing method 
	return json_node;
}


/**
 * Loop JSON text character by character, find text between {...} or [...], send to string parser and put data to map
 *
 * @return map of JSONNode objects.
 */

JSONReader :: JSONNode * JSONReader :: parseObjectOrArray(){
	int parsing_started = 0, parsing_finished = 0, string_started = 0, array_index = 0;
	std::string temp_data_index = "", type = "", sub_return_type = "", parsing_string = "";
	char finish_character, previous_character;
	while (parsing_finished == 0){ // Loop while variable parsing_finished equal 0, else stop the loop
		if(json_from_file == 1){ // If global variable json_from_file is equal 1
			json_file.get(current_character); // Then read next character from file
		}else{
			JSONReader :: readJSONText(); // Else read next character from string
		}
		if(current_character != '\n' && (current_character != ' ' || string_started == 1)){ // Skip new line character and space character if string is bewteen "..."
			std::string character_string(1, current_character); // char convert to string
			if((current_character == '{' || current_character == '[') && string_started == 0){
				if(parsing_started == 0){
					parsing_started = 1;
					if(current_character == '{' && string_started == 0){ // If current character is {, it means we will start parsing object
						type = "object";
						finish_character = '}'; // end of the json object string must finish with character }
					}else if(current_character == '[' && string_started == 0){ // else if current character is [, it means we will start parsing array
						type = "array";
						finish_character = ']'; // end of the json array string must finish with character ]
						parsing_string.append("0:"); // adding default key for the first array item
					}
				}else{
					if(json_from_file == 1){
						json_file.unget(); // go back by one character in file
					}else{
						current_character_index = current_character_index - 2; // go back by two characters in string
						JSONReader :: readJSONText(); // and read next character again
					}
					JSONNode *temp_json_node = JSONReader :: parseObjectOrArray();
					if(temp_json_node->type == "object"){
						object_ref_index = object_ref_index + 1;
						temp_data_index = "object_" + std::to_string(object_ref_index); // generating temporary object data index name
					}else if(temp_json_node->type  == "array"){
						array_ref_index = array_ref_index + 1;
						temp_data_index = "array_" + std::to_string(array_ref_index); // generating temporary array data index name
					}
					temp_data[temp_data_index] = temp_json_node; // put data in temporary map
					parsing_string.append(temp_data_index);
				}
			}else if(current_character == finish_character && string_started == 0){
				parsing_started = 0;
				parsing_finished = 1;
			}else{
				if(current_character == '"' && previous_character != '\\') string_started = string_started == 1 ? 0 : 1; // If current character is " and wasn't escaped by \ it means string between "..." was started
				parsing_string.append(character_string); // adding current character to prepared for parsing string
				if(current_character == ',' && string_started == 0 && type == "array"){ // if current character , and started parsing array, it means we are going to the next array item.
					array_index = array_index + 1;
					std::string array_index_string = std::to_string(array_index);
					parsing_string.append(array_index_string+":");
				}
			}
		}
		previous_character = current_character; // Adding current character to history, for extra information which character was before
	}	
	JSONNode *json_node = JSONReader :: parseJSONString(type, parsing_string); // Send prepared string to parsing function
	json_node->type = type;
	return json_node;
}


/**
 * Parse JSON string between {...} or [...]
 *
 * @param type - is this string JSON object or array, str - JSON string 
 * @return map of JSONNode objects.
 */

JSONReader :: JSONNode * JSONReader :: parseJSONString(std::string type, std::string str){
	JSONNode *parent_json_node = new JSONNode;
	parent_json_node->type = type;
	if(str != ""){
		int string_mode = 0; // if text between double quotes 
		char previous_character;
		std::vector<std::string> objects_values_strings_list;
		std::string object_value_string = "";
		for(int i = 0; i < str.size(); ++i) { // loop all characters in string
			std::string c_string(1, str[i]); // convert char to string
			if(str[i] == '"' && previous_character != '\\') string_mode = string_mode == 1 ? 0 : 1; // If current character is " and wasn't escaped by \ it means string between "..."
			if(str[i] != ',' || string_mode == 1) object_value_string.append(c_string); // if , inside "..." add to value
			if(str[i] == ',' && string_mode == 0){ // if , outside "...", use like separator
				objects_values_strings_list.push_back(object_value_string); // put key:value item in the map
				object_value_string = ""; // prepare generate new string
			}
			previous_character = str[i];  
		}
		objects_values_strings_list.push_back(object_value_string); // parse string to list of key:value items 
		object_value_string = "";
	    for(int i = 0; i < objects_values_strings_list.size(); i++){
	    	JSONNode *json_node = JSONReader :: parseJSONObjectAndValue(objects_values_strings_list[i]); // send key:value items to parse 
	    	parent_json_node->childs[json_node->key] = json_node;
	    	parent_json_node->childs_order.push_back(json_node->key); 
	    }
	}
	return parent_json_node;
}


/**
 * Parse JSON string by format key:value and add extra information about value
 *
 * @param values Container whose values are summed.
 * @return map of JSONNode objects.
 */

JSONReader :: JSONNode *JSONReader :: parseJSONObjectAndValue(std::string str){
	std::string temp_data_index = "";
	char previous_character;
	std::string key = "", value = "", type = "";
	int value_started = 0, string_mode = 0;  // string_mode - if text between double quotes 
	for(int i = 0; i < str.size(); ++i) {  // loop all characters in string
		std::string c_string(1, str[i]); // convert char to string
		if(str[i] == '"' && previous_character != '\\') string_mode = string_mode == 1 ? 0 : 1;
		if(str[i] == ':' && string_mode == 0) value_started = 1; // if current character is : it means string of value was started
		if(value_started == 1){
			if(str[i] == '"' ) type = "string";
			if((str[i] != ':' || string_mode == 1) && (str[i] != '"' || previous_character == '\\')) value.append(c_string);
		}else{
			if(str[i] != '"') key.append(c_string);
		}
		previous_character = str[i];  
	}
	if(type != "string"){ // if value is without " it means this is not string and need to get extra information about it
		if(value == "true" || value == "false"){ // if value true of false, JSON object type will be boolean
			type = "boolean";
			value = value == "true" ? "1" : "0"; // change value true to 1 and false to 0 
		}else if(JSONReader :: isNumber(value)){ 
			type = "number";
		}else if(JSONReader :: isFloat(value)){
			type = "float";
		}else if(value == "null"){
			type = "empty";
		}else if(value.substr(0, 7) == "object_"){ // if value was another object
			type = "object";
		}else if(value.substr(0, 6) == "array_"){ // if value was another array
			type = "array";
		}
	}
	JSONNode *json_node = new JSONNode;
	json_node->key = key;
	json_node->type = type;
	json_node->value = value;
	if(type == "object" || type == "array"){ // if type is object or array, need to find item in temporary data by key and replace
		std::unordered_map<std::string, JSONNode*> ::iterator it;
		for(int i = 0; i < temp_data[value]->childs_order.size(); i++){
			json_node->childs[temp_data[value]->childs_order[i]] = temp_data[value]->childs[temp_data[value]->childs_order[i]];
			json_node->childs_order.push_back(temp_data[value]->childs_order[i]); 
		}
		it = temp_data.find (value); // Find json object/array in temp_data map
		temp_data.erase (it); // and remove it
		json_node->value = "";
	}
	return json_node;
}


/**
 * Check if string is number
 *
 * @param str - string for checking
 * @return True of false
 */

bool JSONReader :: isNumber(const std::string& str){
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}


/**
 * Check if string is float number
 *
 * @param str - string for checking
 * @return True of false
 */

bool JSONReader :: isFloat(std::string str){
	float f;
    std::istringstream iss(str);
    iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
    return iss.eof() && !iss.fail(); // Check the entire string was consumed and if either failbit or badbit is set
}


/**
 * Assign next character of JSON string
 */

void JSONReader :: readJSONText(){ 
	current_character_index = current_character_index + 1;
	current_character = json_text[current_character_index]; // assign specific character from parsing string
}