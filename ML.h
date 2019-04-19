#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include "csv.h"

//data, row
struct Dr{
	std::string data = "";
	int row;
	double price;
	Dr(std::string dat, int r, int p):data(dat), row(r), price(p){}
	Dr(){};
	int operator==(Dr x){
		if ((this->data==x.data)&&(this->row==x.row)){
			return 1;
		}
		return 0;
	}
};

typedef std::vector<std::string> Vstr;
typedef std::vector<std::vector<std::string>> Vvs;
typedef std::map<std::string, std::string> Mss;
typedef std::map<std::string, std::vector<std::string>> Msvs;
typedef std::map<std::string, double> Msd;
typedef std::map<std::vector<std::string>, std::vector<std::string>> Mvsvs;
typedef std::map<std::vector<std::string>, std::vector<Dr>> Mvsvd;

struct ReturnStruct{
	double xtotal = 0;
	double y = 0;
	double b = 0;
	std::vector<double> xdata = std::vector<double>();
	std::vector<double> ydata = std::vector<double>();

};

template <class Obj, class Iter>
int contain(Obj a, Iter b){
	for (Obj x : b){
		if (a==x){
			return 1;
		}
	}
	return 0;
}

template <class key, class val>
std::vector<key> mapKeys(std::map<key, val> m){
	std::vector <key> v = std::vector <key>();
	for (auto const &imap : m){
		v.push_back(imap.first);
	}
	return v;
}

template <class Obj, class Iter>
void printIter(Obj anyobject, Iter iter){
	std::cout << "{";
	for (Obj o: iter){
		std::cout << o << ", ";
	}
	std::cout << "}\n";
}

int maxPriceDr(std::vector<Dr> vdr){
	int row = 0;
	int max = 0;
	for (int i = 0; i < vdr.size(); i++){
		if (vdr[i].price > max){
			max = vdr[i].price;
			row = i;
		}
	}
	return row;
}

Vstr column_count = {"food", "water_in_liter", "electric_in_kwh", "internet_in_gb", "cloth_wash_in_kg"};


template <class Iter>
double sum(Iter iter){
	double result = 0;
	for (double x: iter){
		result+=x;
	}
	return result;
}

template <class Key>
double sumMap(std::map<Key, double> m){
	double result = 0;
	for (auto const &imap : m){
		result+=(imap.second);
	}
	return result;
}


ReturnStruct Start()
{	
	std::cout << "Reading the data...\n";

	//preparing the data
	FILE * fileptr = fopen("id_live_cost.csv", "r");
	CSV csv = CSV(fileptr, "m");

	//finding all unique data
	Msvs unique_data = Msvs();

	//initialize member of unique_data
	for (std::string column : column_count){
		unique_data[column] = Vstr();
	}

	//present the food name as id (number)
	//mapper for food and id
	Msd food_mapper = Msd();

	//identify the food.
	//because this temp0 variable can consume huge memory,
	//we will use dynamic memory so later we can delete it (but with this data, it will only take few megabytes :v)
	Mvsvd *temp0 = new Mvsvd();
	Vstr temp1;
	Vstr the_other = {"water_in_liter", "electric_in_kwh", "internet_in_gb", "cloth_wash_in_kg"};
	std::string temp2 = "";
	int find_food = 1, current_row = 0;

	
	for (Mss row : csv.csv_data_map){		
		for (std::string column : column_count){
			std::string dat = row[column];
			//find unique data
			if (!contain(dat, unique_data[column])){
				unique_data[column].push_back(dat);
			}	
		}
	}

	//asking for input based from unique_data
	int temp;
	Msd choices = Msd();	
	std::string food_choice;
	Mss messages = {
		{"water_in_liter", "liter of waters"},
		{"electric_in_kwh", "KWH of electric"},
		{"internet_in_gb", "GB of internet"},
		{"cloth_wash_in_kg", "KG of clothes"}
	};
	//choices["food"] = unique_data["food"][0];
	// food_choice = unique_data["food"][4];
	// choices["water_in_liter"] = 0.8;
	// choices["electric_in_kwh"] = 1;
	// choices["internet_in_gb"] = 0.2;
	// choices["cloth_wash_in_kg"] = 0.9;
	for (std::string col : mapKeys(unique_data)){
		if (col=="food"){
			int choice;
			int index = 0;
			for (std::string i : unique_data[col]){
				std::cout << index << ". " << i << "\n";
				index++;
			}
			std::cout << "What is your lunch? (input the number): ";
			std::cin >> choice;
			std::cout << "\n";
			food_choice = unique_data["food"][choice];
			std::cout << "\n\n";
			continue;
		}
		std::cout << "How many " << messages[col] << " do you use in a day: ";
		std::cin >> choices[col];
	}

	//Linear regression least square method for each component
	std::cout << "Predicting using Linear Regression Least Square Method....\n";

	std::string cost_rupiah = mapKeys(csv.csv_data_map[0])[1];
	for (int current_row = 1; current_row < csv.csv_data_map.size(); current_row++){
		Mss row = csv.csv_data_map[current_row];		
		temp1 = Vstr();
		//identify the food
		std::string food = row["food"];
		for (std::string other : the_other){
			temp1.push_back(other);
		}
		Dr topush = Dr(food, current_row, std::stod(row[cost_rupiah]));
		if (!contain(topush, (*temp0)[temp1])){
			(*temp0)[temp1].push_back(topush);
		}
		//check if the data is complete as necessary
		if ( ((*temp0)[temp1]).size()==unique_data["food"].size()){
			break;
		}
	}

	//sort the food price
	Vvs keys = mapKeys((*temp0));
	std::vector<Dr> food_dat = (*temp0)[keys[keys.size()-1]];
	std::vector<Dr> food_dat1 = (*temp0)[keys[keys.size()-1]];
	int food_dat_size = food_dat.size();
	int max_index;
	std::vector<Dr> food_sort = std::vector<Dr>();
	for (int i = 0; i < food_dat_size; i++){
		max_index = maxPriceDr(food_dat);
		food_sort.push_back(food_dat[max_index]);
		food_dat.erase(food_dat.begin()+max_index);
	}

	//initialize the food_mapper
	for (int i = 0; i < food_sort.size(); i++){
		Dr dr = food_sort[i];
		food_mapper[dr.data] = food_sort.size()-i;
		//std::cout << dr.data + " : " << food_mapper[dr.data] << "\n";
	}
	delete temp0;

	//initialize the food choice from food_map
	choices["food"] = food_mapper[food_choice];
	// std::cout << "food name: " << food_choice << "|\n";
	// std::cout << "food value: " << choices["food"] << "\n";

	//finding the columns mean
	Msd data_mean = Msd();
	double data_row = csv.csv_data_map.size();
	Vstr column_mean = {"water_in_liter", "electric_in_kwh", "internet_in_gb", "cloth_wash_in_kg", cost_rupiah};
	for (Mss row : csv.csv_data_map){
		data_mean["food"]+=food_mapper[row["food"]]/data_row;
		for (std::string c : column_mean){
			data_mean[c] += std::stod(row[c])/data_row;
		}
	}

	
	// for (std::string key : mapKeys(data_mean)){
	// 	std::cout << key << "\n";		
	// 	std::cout << std::fixed << data_mean[key] <<  " \\ " << data_row << "\n";
	// 	//data_mean[key]/=data_row;
	// 	std::cout << std::fixed << data_mean[key] << "\n\n";
	// }

	//find the slope
	Msd data_slope = Msd();
	Msd data_slope_divider = Msd();
	Vstr column_slope = {"water_in_liter", "electric_in_kwh", "internet_in_gb", "cloth_wash_in_kg"};
	//food is not included because we need to process food with a little bit different

	//in python:
	//for each column:
		//sum((x[i]-x_mean)*(y[i]-y_mean) for i in range(len(x))) <-cost
		// -----------------(Over)--------------------
		//sum((x[i]-x_mean)**2 for i in range(len(x))) <- cost divider	
	
	double temp4, temp5;
	for (Mss row : csv.csv_data_map){
		//implementing sum((x[i]-x_mean)*(y[i]-y_mean) / sum((x[i]-x_mean)**2 for all columns

		//(y[i]-y_mean)
		temp4 = std::stod(row[cost_rupiah])-data_mean[cost_rupiah];
		for (std::string col : column_slope){
			//(x[i]-x_mean)
			temp5 = (std::stod(row[col])-data_mean[col]);

			//(x[i]-x_mean)*(y[i]-y_mean)
			data_slope[col]+=temp5*temp4;

			//sum((x[i]-x_mean)**2
			data_slope_divider[col] += pow(std::stod(row[col])-data_mean[col], 2);
		}
		//handle the food
		temp5 = (food_mapper[row["food"]]-data_mean["food"]);
		data_slope["food"]+=temp5*temp4;
		data_slope_divider["food"] += pow(food_mapper[row["food"]]-data_mean["food"], 2);
	}
	for (auto pair : data_slope){
		data_slope[pair.first]/=data_slope_divider[pair.first];
	}

	//m = the mean of (slope/column_slope)
	double m = sumMap(data_slope)/data_slope.size();

	//else_mean is data_mean without cost_rupiah
	Msd else_mean0 = data_mean;
	for (auto obj = else_mean0.begin(); obj != else_mean0.end(); ){
		if (obj->first==cost_rupiah){
			obj = else_mean0.erase(obj);
		}
		++obj;
	}

	// //printing their choices;
	// std::cout << "\nYour choices: ";
	// for (auto pair : choices){
	// 	std::cout << (pair.first) << " " << (pair.second) << "\n";
	// }
	// std::cout << "\n";

	double else_mean = sumMap(else_mean0)/else_mean0.size();
	double b = (data_mean[cost_rupiah] - (m*else_mean))/(else_mean0.size());


	//implement y = b + (m0*x_input + m1*z_input)/2
	double increaser = (data_slope["food"]* (choices["food"]) + data_slope["water_in_liter"]*choices["water_in_liter"] + data_slope["electric_in_kwh"]*choices["electric_in_kwh"] + data_slope["internet_in_gb"]*choices["internet_in_gb"] + data_slope["cloth_wash_in_kg"]*choices["cloth_wash_in_kg"]);

	//with, b its not accurate
	double y = increaser;


	std::cout << "COST MEAN: " << data_mean[cost_rupiah] << "\n";
	std::cout << std::fixed << "else_mean: " << else_mean << "\n";
	std::cout << std::fixed << "M: " << m << "\n";
	std::cout << std::fixed << "B: " << b << "\n";
	std::cout << "increaser: " << increaser << "\n";
	std::cout << "Cost: " << y << "\n";

	//return the data and result
	ReturnStruct toreturn = ReturnStruct();
	toreturn.y = y;
	toreturn.xtotal = sumMap(choices);
	toreturn.b = b;
	for (auto row : csv.csv_data_map){
		toreturn.xdata.push_back( food_mapper[row["food"]] + std::stod(row["water_in_liter"]) + std::stod(row["electric_in_kwh"]) + std::stod(row["internet_in_gb"]) + std::stod(row["cloth_wash_in_kg"]));
		toreturn.ydata.push_back( std::stod(row[cost_rupiah]));
	}
	return toreturn;
}

