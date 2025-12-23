#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <limits>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
using namespace std;


static bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
 string toLower(string& input) {
	 for (auto& x : input) {
		 x = tolower(x);
	 }
	return input;
}
class Product {
protected:
	string name;
	double quantity;
	string unit;
	friend class CookBookManager;
	friend istream& operator>>(istream& is, Product& product);

public:
	Product(string name,double quantity,string unit):
		name(name),quantity(quantity),unit(unit){}
	Product() :
		name(""), quantity(0), unit("") { }
	void setName(string name) {
		this->name = name;
	}
	void setQuantity(double quantity) {
		this->quantity = quantity;
	}
	void setUnit(string unit) {
		this->unit = unit;
	}
	string getName() { return name; }
	double getQuantity() { return quantity; }
	string getUnit() { return unit; }
	~Product() {};
};
//read product 
istream& operator>>(istream& is, Product& product) {
	is >> product.name >> product.quantity >> product.unit;
	return is;
}
class Category {
protected:
	string name;
	string description;
	friend class CookBookManager;

public:
	Category(string name,string description):name(name),description(description){}

	void setName(string name) {
		this->name = name;
	}
	void setDescription(string description) {
		this->description = description;
	}

	string getName() { return name; }
	string getDescription() { return description; }
	~Category() {};
};
class RecipeFunctionality {
public:
	virtual void explain() = 0;
	virtual void edit() = 0;
	virtual bool hasProduct(const string& productName) = 0;
	virtual ~RecipeFunctionality() {};

};
class Recipe:public RecipeFunctionality{
protected:
	string name;
	vector<Product> products;
	int difficulty;
	string instructions;
	int timeToMake;
	friend class CookBookManager;
	friend class CookBook;
public:
	Recipe(string name, vector<Product> products,int difficulty,string instructions,int timeToMake):
		name(name),products(products),difficulty(difficulty),instructions(instructions),timeToMake(timeToMake)
	{ }
	Recipe() {
		cout << "What is the name of the recipe:";
		cin.ignore();
		getline(cin, name);
		cout << endl;
		products=readProducts();
		cout << endl;
		cout << "Difficulty:";
		cin >> difficulty;
		cout << endl;

		cout << "How much time does it take:";
		cin >> timeToMake;
		cout << endl;
		cin.ignore();
		cout << "How to make it:";
		getline(cin, instructions);
	}
	void explain() override {
		cout << name << "    " << "Difficulty: " << difficulty << " out of 5" << endl
			<< "Time to make: ~" << timeToMake << "minutes" << endl
			<< "Products:" << endl;
		for (auto& p : products) {
			cout << "- " << p.getName() << " " << p.getQuantity() << p.getUnit() << endl;
		}
		cout << "How to make it: " << instructions << endl;
		Sleep(8000);
		system("cls");


	}
	vector<Product> readProducts() {
		vector<Product> current;
		cout << "How many products are you going to add: ";
		int nProducts;
		cin >> nProducts;
		cin.ignore();
		for (int i = 0; i < nProducts; i++) {
			Product p{};
			string name;
			double quantity;
			string unit;
			cout << endl;
			cout << "Product name:";
			getline(cin, name);
			cout << "Product quantity: ";
			cin >> quantity;
			cout << "Product unit: ";
			cin >> unit;
			cin.ignore();
			p.setName(name);
			p.setQuantity(quantity);
			p.setUnit(unit);
			current.push_back(p);
		}
		return current;
	}
	void edit() override {
		cout << "Which aspect of the recipe do you want to edit:" << endl
			<< "(1)Name\n(2)Products list\n(3)Difficulty\n(4)Instructions\n(5)Time to make\n";
		string input;
		bool valid = 1;
		getline(cin, input);
		//cout << "Debug: You entered '" << input << "' (length: " << input.length() << ")" << endl;
		//Sleep(5000);
		if (input == "1") {
			string newName;
			cout << "New name: ";
			getline(cin, newName);
			name = newName;
		}
		else if (input == "2") {
			cout << "Let's make a new products list\n";
			vector<Product> newList =readProducts();
			products = newList;
		}
		else if (input == "3") {
			cout << "What is the new difficulty (1-5): ";
			int newDiff;
			cin >> newDiff;
			difficulty = newDiff;
		}
		else if (input == "4") {
			string newInstructions;
			cout << "What are the new instructions of making this recipe: ";
			getline(cin, newInstructions);
			instructions = newInstructions;
		}
		else if (input == "5") {
			int newTime;
			cout << "What is the new time to make: ";
			cin >> newTime;
			timeToMake = newTime;
		}
		else {
			cout << "Invalid selection" << endl;
			Sleep(2000);
			return;
			valid = 0;
		}
		if (valid)
			cout << "Successful correction of the recipe!" << endl;

		Sleep(2500);
	}
	bool hasProduct(const string& productName) override {
		bool hasProduct = 0;
		for (auto& p : products) {
			if (p.getName() == productName)
				hasProduct = 1;
		}
		return hasProduct;

	}
	~Recipe() {};
};
class CookBookFunctionality {
public:
	virtual void displayContent() = 0;
	virtual void addRecipe() = 0;
	virtual void deleteRecipe(const string& name) = 0;
	virtual void editRecipe(const string& recipeName) = 0;
	virtual void searchRecipeForProduct() = 0;
	virtual void filterByTime() = 0;
};
class CookBook :public CookBookFunctionality	 {
protected:
	string name;
	Category category;
	vector<Recipe> recipes;
	int rating;
	friend class CookBookManager;
public:
	CookBook(string name, Category category, vector<Recipe> recipes, int rating) :
		name(name), category(category), recipes(recipes), rating(rating)
	{ }
	void displayContent() override {
		string input;
		do {
			for (int i = 0; i < recipes.size(); i++) {
				cout << "(" << i + 1 << ") " << recipes[i].name
					<< "    Difficulty: " << recipes[i].difficulty
					<< "    Time: " << recipes[i].timeToMake << " minutes" << endl;
			}
			cout << "Type the number of the recipe you liked to see the instructions or Type:\nPRODUCT to search by it" << endl
				<<"TIME to filter through it\nADD to add a recipe\nDELETE to delete a recipe\nEDIT to edit a recipe\nBACK to return to the books" << endl;
			cin >> input;
			input=toLower(input);
			if (is_number(input)) {
				int selection = stoi(input);
				if (selection < 1 || selection > recipes.size()) {
					cout << "Invalid selection! Try Again!" << endl;
					Sleep(2000);
					system("cls");
					continue;
				}
				else {
					recipes[selection - 1].explain();
					continue;
				}
			}
			else if (input == "product") {
				searchRecipeForProduct();
			}
			else if (input == "time") {
				filterByTime();
			}
			else if (input == "add") {
				addRecipe();
				system("cls");
			}
			else if (input == "delete") {
				string recipeName;
				cout << "Which recipe do you want to delete(give its exact name): ";
				cin.ignore();
				getline(cin, recipeName);
				deleteRecipe(recipeName);
				system("cls");
			} 
			else if (input == "edit") {
				string recipeName;
				cout << "Which recipe do you want to edit(give its exact name): ";
				cin.ignore();
				getline(cin, recipeName);
				editRecipe(recipeName);
				system("cls");
			}
			else if (input == "back") {
				cin.ignore();
				system("cls");
				break;
			}
			else {
				cout << "Invalid input! Try again!" << endl;
				Sleep(2000);
				system("cls");
			}
		} while (true);
	}
	void addRecipe()override {
		Recipe re{};
		recipes.push_back(re);
	}
	void deleteRecipe(const string& name)override {
		for (auto i = recipes.begin(); i != recipes.end();) {
			if ((i)->name == name) {
				i = recipes.erase(i);
			}
			else {
				++i;
			}
		}
	}
	void editRecipe(const string& recipeName)override {
		bool invalidInput = 1;
		for (auto& recipe : recipes) {
			if (recipe.name == recipeName) {
				recipe.edit();
				invalidInput = 0;
				continue;
			}
		}
		if (invalidInput)
			cout << "Invalid Recipe Name" << endl;
		Sleep(2000);
	}
	void searchRecipeForProduct() override {
		system("cls");
		string productName;
		cout << "Which product do you have: ";
		cin.ignore();
		getline(cin, productName);
		bool atleastOneHasThisProduct = 0;
		for (auto& r : recipes) {
			if (r.hasProduct(productName)) {
				cout << "This recipe has got this product: " << r.name << endl;
				atleastOneHasThisProduct = 1;
				Sleep(1000);
			}
		}
			if (!atleastOneHasThisProduct) {
				cout << "No recipe has this product sorry!" << endl;
				Sleep(1500);
			}

	}
	void filterByTime() override {
		int availableTime = 0;
		bool hasNoTimeForRecipe = 1;
		cin.ignore();
		system("cls");
		cout << "How much time do you have for cooking:";
		cin >> availableTime;
		cout << endl;
		for (auto& recipe : recipes) {
			if (recipe.timeToMake <= availableTime) {
				cout << "You can do this recipe:"<<recipe.name
					<< "    Difficulty: " << recipe.difficulty
					<< "    Time: " << recipe.timeToMake << " minutes" << endl;
				Sleep(1500);
				hasNoTimeForRecipe = 0;
			}
		}
		if (hasNoTimeForRecipe)
			cout << "Sorry there isn't a recipe that takes less than " << availableTime << " minutes go buy something!" << endl; Sleep(2000);
		


	}
	};

	class CookBookManager {
	private:
		vector<CookBook> cookBooks;
	public:
		CookBookManager() {}
		void start() {
			cout << "Welcome what do you want to eat today?" << endl;
			string input;
			/*CookBook savory("Savory", Category("Savory", "Savory foods"), {}, 5);
			CookBook smoothies("Smoothies", Category("Smoothies", "Food in the form of a smoothie/shake"), {}, 5);
			CookBook baked("Savory", Category("Baked Goods", "Bread cakes and all things made with flour in the oven"), {}, 3);
			cookBooks.push_back(savory);
			cookBooks.push_back(smoothies);
			cookBooks.push_back(baked);*/
			do {
				cout << "Choose a number:" << endl
					<< "(1) Sweet recipes Book		"<<"rating: "<<cookBooks[0].rating<<" out of 5" << endl
					<< "(2) Savory recipes Book		" << "rating: " << cookBooks[1].rating << " out of 5" << endl
					<< "(3) Smoothies Book		" << "rating: " << cookBooks[2].rating << " out of 5" << endl
					<< "(4) Baked Goods Book		" << "rating: " << cookBooks[3].rating << " out of 5" << endl
					<< "(0) Exit " << endl;
				getline(cin, input);
				//sweet
				if (input == "1") {
					system("cls");

					for (auto& book : cookBooks) {
						if (book.category.name == "Sweet") {
							book.displayContent();
						}
					}
				}//savory
				else if (input == "2") {
					system("cls");

					for (auto& book : cookBooks) {
						if (book.category.name == "Savory") {
							book.displayContent();
						}
					}

				}//smoothie
				else if (input == "3") {
					system("cls");

					for (auto& book : cookBooks) {
						if (book.category.name == "Smoothies") {
							book.displayContent();
						}
					}
				}//baked goods
				else if (input == "4") {
					system("cls");

					for (auto& book : cookBooks) {
						if (book.category.name == "Baked Goods") {
							book.displayContent();
						}
					}
				}
				else if (input == "0") {
					system("cls");

					cout << "Goodbye! Have a wonderful day!" << endl;
				}
				else {
					cout << "Invalid selection! Try Again!" << endl;
					Sleep(1000);
					system("cls");
				}



			} while (input != "0");

		}
		//TO DO
		void readBooksFromFile() {
			ifstream file("cookbooks.txt");
			if (file.is_open()) {
				int booksCount;
				file >> booksCount;
				file.ignore(25, '\n');
				for (int i = 0; i < booksCount; i++) {
					string bookName;
					getline(file, bookName);
					string catName; string catDesc;
					getline(file, catName); getline(file, catDesc);
					int rating;
					file >> rating;
					file.ignore(25, '\n');
					Category cat(catName, catDesc);
					int recipesCount;
					file >> recipesCount;
					file.ignore(25, '\n');
					vector<Recipe> recs;
					
					for (int k = 0; k < recipesCount; k++) {
						vector < Product> products;
						string reciName;
						getline(file, reciName);
						int productCount;
						file >> productCount;
						file.ignore(25, '\n');
						for (int j = 0; j < productCount; j++) {
							string pName; double qty; string unit;
							getline(file, pName);
							/*cout << "Prodyct name: " << pName;
							Sleep(1000);*/
							file >> qty;
							file >> unit;
							file.ignore(25, '\n');
							Product p(pName, qty, unit);
							products.push_back(p);
						}
					int difficulty; int timeToMake; string instructions;
					file >> difficulty;
					file >> timeToMake;
					file.ignore(25, '\n');
					getline(file, instructions);
					Recipe r(reciName, products, difficulty, instructions, timeToMake);
					recs.push_back(r);
					}
					CookBook cBook(bookName, cat, recs, rating);
					cookBooks.push_back(cBook);
				}
				file.close();
			}
			else {
				cerr << "Error occured when reading from file!" << endl;
			}

		}
		void writeBooksToFile() {
			ofstream file("cookbooks.txt");
			if (file.is_open()) {
				file << cookBooks.size() << endl;
				for (auto& book : cookBooks) {
					file << book.name << endl;
					file << book.category.getName() << endl << book.category.getDescription() << endl;
					file << book.rating << endl;
					file << book.recipes.size() << endl;
					for (auto& recipe : book.recipes) {
						file << recipe.name << endl; file << recipe.products.size() << endl;
						for (auto& product : recipe.products) {
							file << product.getName() << endl;
							file << product.getQuantity() << endl;
							file<< product.getUnit() << endl;
						}
						file << recipe.difficulty << endl;
						file << recipe.timeToMake << endl;
						file << recipe.instructions << endl;
					}
				}
				file.close();

			}
			else {
				cerr << "Error occured while writing to the file!" << endl;
			}
		}
		~CookBookManager() {
			writeBooksToFile();
		}
	};
	int main() {

		CookBookManager manager;
		manager.readBooksFromFile();
		manager.start();

		return 0;
	}