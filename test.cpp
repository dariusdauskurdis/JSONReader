#include <iostream>
using namespace std;

#include "JSONReader.h"

int main()
{
	
	std::cout << "*** TEST 1 ***\n";
	std::cout << "Reading JSON data from file\n";

	JSONReader *json_reader_1 = new JSONReader;
	std::map<std::string, JSONReader :: JSONNode*> json_data_1 = json_reader_1->loadFromFile("test.json");

	std::cout << json_data_1["country"]->value << "\n";

	std::cout << json_data_1["text with quotes"]->value << "\n";

	std::map<std::string, JSONReader :: JSONNode*> list_of_items = json_data_1["array of names"]->childs;
	std::map<std::string, JSONReader :: JSONNode*> ::iterator it;
	for (it = list_of_items.begin(); it != list_of_items.end(); it++)
	{
	    std::cout << "Key: " << it->first << ", value: " << it->second->value << ", type: " << it->second->type << "\n";
	}

	std::cout << json_data_1["array of objects"]->childs["1"]->childs["status"]->type << "\n";


	std::cout << "\n*** TEST 2 ***\n";
	std::cout << "Reading JSON data from string\n";

	JSONReader *json_reader_2 = new JSONReader;
	std::string json_string = "{\"random\":87,\"random float\":43.257,\"bool\":true,\"empty\":null,\"name\":\"Stacey Eugenia\",\"country\":\"Lithuania\",\"text with quotes\":\"John said, \\\"I really happy when it’s hot outside.\\\"\",\"array of names\":[\"Meg\",\"Danika\",\"Kore\"],\"array of objects\":[{\"bool\":false,\"random float\":25.768},{\"random\":28,\"status\":null}],\"extra\":{\"age\":35}}";
	std::map<std::string, JSONReader :: JSONNode*> json_data_2 = json_reader_2->load(json_string);

	std::cout << json_data_2["bool"]->value << "\n";
	std::cout << json_data_2["array of objects"]->childs.size() << "\n";
	std::cout << json_data_2["array of objects"]->childs["0"]->childs["random float"]->type << "\n";
	
    return 1;
}