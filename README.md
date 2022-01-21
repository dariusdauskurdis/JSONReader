# JSONReader
Very simple JSON data parser, which converts JSON string/file to map of objects.
# Compile and run executable file
Open console, go to project directory, compile selected files and run executable file "converter":

```sh
$ cd ~/my_projects/JSONReader
$ g++-7 test.cpp JSONReader.cpp -o converter -std=c++17 -lstdc++fs
$ ./converter
```
# Examples

JSON file "test.json" structure:

```json
{
  "random": 87,
  "random float": 43.257,
  "bool": true,
  "empty": null,
  "name": "Stacey Eugenia",
  "country": "Lithuania",
  "text with quotes": "John said, \"I really happy when it’s hot outside.\"",
  "array of names": [
    "Meg",
    "Danika",
    "Kore"
  ],
  "array of objects": [
    {
      "bool": false,
      "random float": 25.768
    },
    {
      "random": 28,
      "status": null
    }
  ],
  "extra": {
    "age": 35
  }
}
```

First test: to parse JSON file data


```cpp
#include <iostream>
using namespace std;

#include "JSONReader.h"

int main()
{
	std::cout << "*** TEST 1 ***\n";
	std::cout << "Reading JSON data from file\n";

	JSONReader *json_reader_1 = new JSONReader;
	JSONReader :: JSONNode* json_data_1 = json_reader_1->loadFromFile("test.json");


	std::cout << json_data_1->childs["country"]->value << "\n";

	std::cout << json_data_1->childs["text with quotes"]->value << "\n";

	std::map<std::string, JSONReader :: JSONNode*> list_of_items = json_data_1->childs["array of names"]->childs;
	std::map<std::string, JSONReader :: JSONNode*> ::iterator it;
	for (it = list_of_items.begin(); it != list_of_items.end(); it++){
	    std::cout << "Key: " << it->first << ", value: " << it->second->value << ", type: " << it->second->type << "\n";
	}

	std::cout << json_data_1->childs["array of objects"]->childs["1"]->childs["status"]->type << "\n";

	return 1;
}

```

Result:


```bash
*** TEST 1 ***
Reading JSON data from file
Lithuania
John said, \"I really happy when it’s hot outside.\"
Key: 0, value: Meg, type: string
Key: 1, value: Danika, type: string
Key: 2, value: Kore, type: string
empty
```

If need to use JSON data as string, need to minimize JSON data structure in one line, escape double quotes. 

```bash 
{\"random\":87,\"random float\":43.257,\"bool\":true,\"empty\":null,\"name\":\"Stacey Eugenia\",\"country\":\"Lithuania\",\"text with quotes\":\"John said, \\\"I really happy when it’s hot outside.\\\"\",\"array of names\":[\"Meg\",\"Danika\",\"Kore\"],\"array of objects\":[{\"bool\":false,\"random float\":25.768},{\"random\":28,\"status\":null}],\"extra\":{\"age\":35}}
```


Second test: to parse JSON string


```cpp
#include <iostream>
using namespace std;

#include "JSONReader.h"

int main()
{
	std::cout << "\n*** TEST 2 ***\n";
	std::cout << "Reading JSON data from string\n";

	JSONReader *json_reader_2 = new JSONReader;
	std::string json_string = "{\"random\":87,\"random float\":43.257,\"bool\":true,\"empty\":null,\"name\":\"Stacey Eugenia\",\"country\":\"Lithuania\",\"text with quotes\":\"John said, \\\"I really happy when it’s hot outside.\\\"\",\"array of names\":[\"Meg\",\"Danika\",\"Kore\"],\"array of objects\":[{\"bool\":false,\"random float\":25.768},{\"random\":28,\"status\":null}],\"extra\":{\"age\":35}}";
	std::map<std::string, JSONReader :: JSONNode*> json_data_2 = json_reader_2->load(json_string);

	std::cout << json_data_2->childs["bool"]->value << "\n";
	std::cout << json_data_2->childs["array of objects"]->childs.size() << "\n";
	std::cout << json_data_2->childs["array of objects"]->childs["0"]->childs["random float"]->type << "\n";

	return 1;
}

```

Result:


```bash
*** TEST 2 ***
Reading JSON data from string
1
2
float
```

Third test: loop json object by inserted order

```cpp
#include <iostream>
using namespace std;

#include "JSONReader.h"

int main()
{
	std::cout << "\n*** TEST 3 ***\n";
	std::cout << "Loop JSON data by inserted order\n";

	JSONReader *json_reader_2 = new JSONReader;
	std::string json_string = "{\"random\":87,\"random float\":43.257,\"bool\":true,\"empty\":null,\"name\":\"Stacey Eugenia\",\"country\":\"Lithuania\",\"text with quotes\":\"John said, \\\"I really happy when it’s hot outside.\\\"\",\"array of names\":[\"Meg\",\"Danika\",\"Kore\"],\"array of objects\":[{\"bool\":false,\"random float\":25.768},{\"random\":28,\"status\":null}],\"extra\":{\"age\":35}}";
	std::map<std::string, JSONReader :: JSONNode*> json_data_2 = json_reader_2->load(json_string);

	for (int i = 0; i < json_data_2->childs_order.size(); ++i){
		std::string child_key = json_data_2->childs_order[i];
		std::cout << "Key: " << child_key << ", value: " << json_data_2->childs[child_key]->value << ", type: " << json_data_2->childs[child_key]->type << "\n";
	}

	return 1;
}

```

Result:


```bash
*** TEST 3 ***
Loop JSON data by inserted order
Key: random, value: 87, type: number
Key: random float, value: 43.257, type: float
Key: bool, value: 1, type: boolean
Key: empty, value: null, type: empty
Key: name, value: Stacey Eugenia, type: string
Key: country, value: Lithuania, type: string
Key: text with quotes, value: John said, \"I really happy when it’s hot outside.\", type: string
Key: array of names, value: array_1, type: array
Key: array of objects, value: array_2, type: array
Key: extra, value: object_3, type: object
```

Notice that childs are maps, and maps have auto sorting by key, but if need to loop by inserted order need to use class member "childs_order"

The idea of this library was to create something very simple on c++, only few functions, to use for parsing JSON file or string.

**Works only with valid JSON format** 