#include <iostream>
#include <cstdio> 
#include <string>
#include <sstream> 
#include <fstream>
#include <filesystem>  
#include <iomanip>
#include <map>


using namespace std::experimental::filesystem::v1;


long getDirectorySize(const path& p) {
	long size = 0;
	for (const auto& entry : directory_iterator{ p }) {
		if (is_regular_file(entry.status())) {
			size += file_size(entry.path());
		}
		else if (is_directory(entry.status())) {
			std::wstring dir_inside{ entry.path().filename() };
			const path p_inside{ dir_inside };
			size += getDirectorySize(p / p_inside);
		}
	}
	return size;
}


void write(const path& p, const std::string& file_path) {

	std::ofstream output;
	output.open(file_path);
	if (!output.is_open()) {
		return;
	}

	std::map<long, path> data;
	
	output << "Begin iterating " << p.wstring() << std::endl;
	for (const auto& entry : directory_iterator{ p }) {
		if (is_regular_file(entry.status())) {
			data[file_size(entry.path())] = entry.path().filename();
		}
		else if (is_directory(entry.status())) {
			std::wstring dir_inside{ entry.path().filename() };
			const path p_inside{ dir_inside };
			data[getDirectorySize(p / p_inside)] = entry.path().filename();
		}
	}
	
	for (auto it = data.begin(); it != data.end(); ++it) {
		output << "< " << it->first << " > : < " << it->second << " >\n";
	}
	output.close();
}


int main(int argc, const char* argv[]) { 

	setlocale(LC_ALL, "ru");

	if (argc < 3) {
		std::cout << "Invalid argument count \n";
		return 1;
	}
	
	const char * dirName = argv[1];
	path p_dir{ dirName };
	if (!is_directory(p_dir)) {
		std::wcout << L"No such directory: " << dirName << std::endl;
		system("pause");
		return 0;
	}
	
	const char * dstFileName = argv[2];
	std::string str = dstFileName;

	write(p_dir, str);
	std::cout << std::endl;
	
	system("pause");
	return 0;
}

