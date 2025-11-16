#include "ece250_socket.h"
#include <iostream>
#include <sstream> //new include!

int main()
{
	//Just for fun, let's reclassify "Dog"
	std::string text_to_classify = "Dog";
	std::string candidate_labels = "animal,plant,technology,no thing";
	std::cout << labelText(text_to_classify,candidate_labels) << std::endl;

	//OK, now let's split a string. In fact, let's split candidate_labels
	
	//step 1: make a new stringstring with the contents of our candidate_labels 
	std::istringstream stream(candidate_labels); //I could have named it anything, like stringin or something
	
	//step 2: make a temporary string that we will use to store the extracted parts
	std::string single_label;
	
	//step 3: use the getline function with the comma sent in as a delimiter to read the string parts
	while(std::getline(stream,single_label,',')){
		std::cout << single_label << std::endl; //and let's print it just to make sure it worked
	}
	
	return 0;
}
