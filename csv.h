#ifndef __csv
#define __csv true
#include <string> //cpp standard library
#include <vector> //cpp standard library
#include "strtool.h" //find this on my github: https://github.com/KevinAS28
#include <map> //cpp standard library
typedef std::vector<std::string> vstr; //vector string
typedef std::vector<vstr> vvstr; //vector vector string
class CSV;
typedef void (CSV::*readerFunc)(std::string, std::string, std::string);
typedef std::vector<std::map<std::string, std::string>> DataMap;


class CSV{
public:
	//data
	vvstr csv_data_vector = vvstr();
	DataMap csv_data_map = DataMap();

	//configuration
	int pass_error = 1;

	//
	std::map<std::string, readerFunc> readerMapper = std::map<std::string, readerFunc>();
	FILE * csv_file;
	std::string column_splitter = ",";
	std::string row_splitter = "\n";
	std::string type = "a";
	void reader(std::string data, std::string row_splitter="\n",std::string column_splitter = ","){
		vstr line = split_str_s(data, row_splitter);
		this->column_splitter = column_splitter;
		for (std::string i : line){
			vstr dat = split_str_s(i, column_splitter);
			this->csv_data_vector.push_back(dat);
		}
	}
	void mapReader(std::string data, std::string row_splitter="\n",std::string column_splitter = ","){
		vstr line = split_str_s(data, row_splitter);
		if (line.size() <= 1){
			printf("CSV() -> mapReader(): the row num data should be > 1. because the first row is column name");
		}
		this->column_splitter = column_splitter;
		vstr column = split_str_s(line[0], column_splitter);
		std::map<std::string, std::string> toadd;
		for (int a = 1; a < line.size(); a++){
			vstr dat = split_str_s(line[a], column_splitter);
			if (dat.size()!=column.size()){
				
				if (pass_error){
					continue;
				}
				else{
					printf("Error on line %d, with length column: %d  and expecting: %d\n", a, dat.size(), column.size());
				}
			}
			toadd = std::map<std::string, std::string>();
			for  (int b = 0; b < column.size(); b++){

				toadd[column[b]] = dat[b];
			}
			this->csv_data_map.push_back(toadd);
		} 
	}

	void reader(std::string data, std::string type="a", std::string row_splitter="\n",std::string column_splitter = ","){
		this->type = type;
		readerFunc func = (this->readerMapper[type]);
		(this->*func)(data, row_splitter, column_splitter);
	}

	CSV(){
		//initialize the mapper;
		this->readerMapper["a"] = &CSV::reader;
		this->readerMapper["m"] = &CSV::mapReader;
	}

	CSV(std::string data, std::string type="a", std::string row_splitter="\n",std::string column_splitter = ","): CSV(){
		this->reader(data, type, row_splitter, column_splitter);
	}

	CSV(FILE * the_file, std::string type="a", std::string row_splitter="\n",  std::string column_splitter = ","):CSV(){
		std::string data = "";
		while (true){
			int d = getc(the_file);
			if (d==EOF)break;
			data+=d;
		}
		this->reader(data, type, row_splitter, column_splitter);
	}

	std::string get_string(){
		std::string to_return = "";
		for (vstr v: this->csv_data_vector){
			for (std::string s:v){
				to_return+=s;
				to_return+=this->column_splitter;
			}
			to_return+=this->row_splitter;
		}
		return to_return;
	}
};

#endif