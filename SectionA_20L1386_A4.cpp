#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//reads words from a file and copies them in a dynamic array
char** ReadWordsListFromFileToCreateGrid(ifstream& wordsList, int& largest , int totalWords)
{
	char** readString = 0; //to store data and return
	char   tempBuffer[80][80];  //to temproraily store data
	char word[80];

	if (wordsList.is_open())
	{	
		readString = new char*[totalWords]; //dynamicaly alloacte rows 

		for (int index = 0; index < totalWords; index++)
		{
			wordsList.getline(word, 80);
			int totalChar = strlen(word);  //total characters in each word

			readString[index] = new char[totalChar+1];  //dynamicaly alloacte coloumns on each row

			if (totalChar > largest)
				largest = totalChar;  //grid is of the size of largest word

			for (int charIndex = 0; charIndex < totalChar; charIndex++)
			{
				readString[index][charIndex] = word[charIndex];  //copy from temporary to dynamic array
			}
			readString[index][totalChar] = '\0';

		}
		wordsList.close();
		cout << "words loaded successfully" << endl;
		return readString;
	}
	else
	{
		cout << "Error ! Enter correct file";
		return 0;
	}
}

//reads names from a file and copies them in a dynamic array
char** ReadDataFromFileToSearchWords(ifstream& dataFile, int& gridRows , int& gridCols , int& totalWords)
{
	char** readString = 0;
	char   tempBuffer[80];  //to store data temporarily

	if (dataFile.is_open())
	{
		dataFile >> gridRows;
		dataFile >> gridCols;
		dataFile >> totalWords;   //total names in the file
		dataFile.ignore();  //to ignore garbage value and enable loop to work till last name

		if (totalWords > 0)  //validity check
		{
			readString = new char* [totalWords];  //rows allocation

			for (int index = 0; index < totalWords; index++)
			{
				int totalChar = 0;
				dataFile.getline(tempBuffer, 80);  //input data from file
				int position = 0;
				while (tempBuffer[position] != '\0')
				{
					totalChar++;
					position++;
				}

				readString[index] = new char[totalChar+1];  //cols allocation

				for (int charIndex = 0; charIndex < totalChar; charIndex++)
				{
					readString[index][charIndex] = tempBuffer[charIndex];
				}
				readString[index][totalChar] = '\0';
			}

			dataFile.close();
			cout << "Words loaded successfully" << endl;
			return readString;
		}
		else
		{
			cout << "Error ! The file does not have rows more than zero" << endl;
			return 0;
		}
	}
	else
	{
		cout << "Error ! Enter correct file";
		return 0;
	}
}

//takes size and array from the file
char** InputGrid(ifstream& dataFile, int& rows , int& cols)
{
	char** myGrid = 0;

	if (dataFile.is_open())    //check if entered file exists
	{
		if (rows > 0 && cols > 0)
		{
			myGrid = new char* [rows];   
			char** endRows = myGrid + rows;

			for (char** index = myGrid; index < endRows; index++)     //allocates memory for 2D array
				*index = new char [cols+1]; 

			for (char** i = myGrid ; i < endRows ; i++)
			{
				char* endCols = *i + cols;
				for (char* j = *i ; j < endCols ; j++)
					dataFile >> *j;       //takes data from file
			}
		}
		dataFile.close();
		cout << "Grid loaded successfully" << endl;
		return myGrid;
	}
	else
		cout<<"Please enter the correct file";
	return 0;
}

//creates an grid to store words
char** SearchGrid(char** grid , int gridRows , int gridCols)
{
	grid = new char*[gridRows+1];

	for (int i = 0; i < gridRows; i++)
	{
		grid[i] = new char[gridCols+1];

		for (int j = 0; j < gridCols; j++)
		{
			grid[i][j] = '-';
		}
		grid[i][gridCols] = '\0';  
	}
	grid[gridRows] = new char[1];
	grid[gridRows][0] = '\0';

	return grid;
}

bool HorizontallyForwardPlacing(char**& grid, char* searchWord, int totalRows, int totalCols )
{
	int count, gridRows = 0, gridCols = 0;
	int wordLength = strlen(searchWord);

	//whole grid loop
	//totalWords is the grid size

	for (gridRows = 0; gridRows < totalRows; gridRows++)
	{
		for (gridCols = 0; gridCols < totalCols; gridCols++)
		{
			//horizontally forward
			count = 0;
			int forwardRows = gridRows, forwardCols = gridCols;
			for (int charIndex = 0; charIndex < wordLength && forwardCols < totalCols; charIndex++)
			{
				if (grid[forwardRows][forwardCols] == '-')
				{
					count++;
					forwardCols++;
				}
				else
					charIndex += wordLength;
			}
			if (count == wordLength)
			{
				forwardCols = forwardCols - (wordLength);
				for (int c = 0; c < wordLength; c++)
				{
					grid[gridRows][forwardCols] = searchWord[c];
					forwardCols++;
				}
				return true;
			}
		}

	}

	return false;
}

//this function places the words horizontally backwards in the grid
bool HorizontallyBackwardsPlacing(char**& grid, char* searchWord, int totalRows , int totalCols)
{
	int count, gridRows = 0, gridCols = 0;
	int wordLength = strlen(searchWord);

	//whole grid loop
	//totalWords is the grid size

	for (gridRows = 0; gridRows < totalRows; gridRows++)
	{
		for (gridCols = 0; gridCols < totalCols; gridCols++)
		{
			//horizontally backwards
			count = 0;
			int backRows = gridRows, backCols = gridCols;
			for (int charIndex = 0; charIndex < wordLength && backCols > 0; charIndex++)
			{
				if (grid[backRows][backCols] == '-')
				{
					count++;
					backCols--;
				}
				else
					charIndex += wordLength;
			}
			if (count == wordLength)
			{
				backCols = backCols + (wordLength);
				for (int c = 0; c < wordLength; c++)
				{
					grid[gridRows][backCols] = searchWord[c];
					backCols--;
				}
				return true;
			}
		}

	}

	return false;
}


//this function places the words vertically downwards in the grid
bool VerticallyDownwardsPlacing (char** &grid , char* searchWord , int gridRows , int gridCols)
{
	int count;
	int wordLength = strlen(searchWord);

	//whole grid loop
	
	for (int rows = 0; rows < gridRows; rows++)
	{
		for (int cols = 0; cols < gridCols; cols++)
		{
			//vertical downwards
			count = 0;
			int forwardRows = rows, forwardCols = cols;
			for(int charIndex = 0; charIndex < wordLength && forwardRows < gridRows; charIndex++)
			{
				if (grid[forwardRows][forwardCols] == '-')
				{
					count ++;
					forwardRows++;
				}
				else
					charIndex+= wordLength;
			}
			if (count == wordLength)
			{
				forwardRows = forwardRows - (wordLength);
				for (int c = 0; c < wordLength; c++)
				{
					grid[forwardRows][forwardCols] = searchWord[c];
					forwardRows++;
				}
				return true;
			}
		}

	}
	return false;
}

//this function places the words vertically upwards in the grid
bool VerticallyUpwardsPlacing (char** &grid , char* searchWord , int gridRows , int gridCols)
{
	int count;
	int wordLength = strlen(searchWord);

	//whole grid loop

	for (int rows = 0; rows < gridRows; rows++)
	{
		for (int cols = 0; cols < gridCols; cols++)
		{
			//vertically upwards
			count = 0;
			int backRows = rows , backCols = cols;
			for(int charIndex = 0; charIndex < wordLength && backRows >= 0; charIndex++)
			{
				if (grid[backRows][backCols] == '-')
				{
					count ++;
					backRows--;
				}
				else
					charIndex+= wordLength;
			}
			if (count == wordLength)
			{
				backRows = backRows + (wordLength); 
				for (int c = 0; c < wordLength; c++)
				{
					grid[backRows][backCols] = searchWord[c];
					backRows--;
				}
				return true;
			}
		}

	}
	return false;
}

//this function places the words left to right diagonally downwards in the grid
bool DiagonalOnePlacing (char** &grid , char* searchWord , int gridRows , int gridCols)
{
	int count = 0;
	int wordLength = strlen(searchWord);  

	//whole grid loop
	for (int rows = 0; rows < gridRows; rows++)
	{
		for (int cols = 0; cols < gridCols; cols++)
		{
			//left to right downwards
			count = 0;
			int rows_D1 = rows , cols_D1 = cols;  
			for(int charIndex = 0; charIndex < wordLength &&  rows_D1 < gridRows &&  cols_D1 < gridCols; charIndex++) 
			{
				if (grid[rows_D1][cols_D1] == '-')
				{
					count ++;
					rows_D1++;
					cols_D1++;
				}
				else
					charIndex+= wordLength;
			}
			if (count == wordLength)
			{
				rows_D1 = rows_D1 - wordLength;
				cols_D1 = cols_D1 - wordLength;
				for (int c = 0; c < wordLength; c++)
				{
					grid[rows_D1][cols_D1] = searchWord[c];
					rows_D1++;
					cols_D1++;
				}
				return true;
			}	

		}
	}
	return false;
}

//this function places the words left to right diagonally upwards in the grid
bool DiagonalTwoPlacing (char** &grid , char* searchWord , int gridRows , int gridCols)
{
	int count = 0;
	int wordLength = strlen(searchWord);  

	//whole grid loop
	for (int rows = 0; rows < gridRows; rows++)
	{
		for (int cols = 0; cols < gridCols; cols++)
		{
			//left to right upwards
			count = 0;
			int rows_D2 = rows , cols_D2 = cols;  
			for(int charIndex = 0; charIndex < wordLength &&  rows_D2 >= 0 &&  cols_D2 < gridCols ; charIndex++)
			{
				if (grid[rows_D2][cols_D2] == '-') 
				{
					count ++;
					rows_D2--;
					cols_D2++;
				}
				else
					charIndex+= wordLength;

			}
			if (count == wordLength)
			{
				rows_D2 = rows_D2 + wordLength;
				cols_D2 = cols_D2 - wordLength;
				for (int c = 0; c < wordLength; c++)
				{
					grid[rows_D2][cols_D2] = searchWord[c];
					rows_D2--;
					cols_D2++;
				}
				return true;
			}
		}
	}
	return false;
}

//this function places the words right to left diagonally downwards in the grid
bool DiagonalThreePlacing (char** &grid , char* searchWord ,int gridRows , int gridCols)
{
	int count = 0;
	int wordLength = strlen(searchWord);  

	//whole grid loop
	for (int rows = 0; rows < gridRows; rows++)
	{
		for (int cols = 0; cols < gridCols; cols++)
		{
			//right to left downwards
			count = 0;
			int rows_D3 = rows , cols_D3 = cols; 
			for(int charIndex = 0; charIndex < wordLength &&  rows_D3 < gridRows &&  cols_D3 >= 0; charIndex++)
			{
				if (grid[rows_D3][cols_D3] == '-')
				{
					count ++;
					rows_D3++;
					cols_D3--;
				}
				else
					charIndex+= wordLength;
			}
			if (count == wordLength)
			{
				rows_D3 = rows_D3 - wordLength;  
				cols_D3 = cols_D3 + wordLength;
				for (int c = 0; c < wordLength; c++)
				{
					grid[rows_D3][cols_D3] = searchWord[c];
					rows_D3++;
					cols_D3--;
				}
				return true;
			}
		}
	}
	return false;
}

//this function places the words right to left diagonally upwards in the grid
bool DiagonalFourPlacing (char** &grid , char* searchWord , int gridRows , int gridCols)
{
	int count = 0;
	int wordLength = strlen(searchWord);  

	//whole grid loop
	for (int rows = 0; rows < gridRows; rows++)
	{
		for (int cols = 0; cols < gridCols; cols++)
		{
			//right to left upwards
			count = 0;
			int rows_D4 = rows , cols_D4 = cols;

			for(int charIndex = 0; charIndex < wordLength &&  rows_D4 >= 0 &&  cols_D4 >= 0; charIndex++)
			{
				if (grid[rows_D4][cols_D4] == '-')
				{
					count ++;
					rows_D4--;
					cols_D4--;
				}
				else
					charIndex+= wordLength;
			}
			if (count == wordLength)
			{
				rows_D4 = rows_D4 + wordLength;
				cols_D4 = cols_D4 + wordLength;
				for (int c = 0; c < wordLength; c++)
				{
					grid[rows_D4][cols_D4] = searchWord[c];
					rows_D4--;
					cols_D4--;
				}
				return true;
			}
		}
	}
	return false;
}

//this function checks if the word is present horizontally forward in the grid
bool HorizonallyForwardCheck(char* searchWord, int size, char** Grid, int gridRows, int gridCols, int*& position)
{
	for (int row = 0; row < gridRows; row++)
	{
		for (int col = 0; col < gridCols; col++)
		{
			if (searchWord[0] == Grid[row][col])    //first alphabet check
			{

				int wordIndex = 0, gridIndex = col, count = 0;

				//horizontally forward
				while (gridIndex < gridCols)
				{
					if (searchWord[wordIndex] == Grid[row][gridIndex])
					{
						count++;
						wordIndex++;
						gridIndex++;
					}
					else
						gridIndex += gridCols;  //to end the loop
				}

				if (count == size)
				{
					*(position) = row;
					*(position + 1) = col;
					*(position + 2) = row;
					*(position + 3) = col + (size - 1);

					return true;
				}
			}
		}
	}
	return false;
}

//this function checks if the word is present horizontally backwards in the grid
bool HorizonallyBackwardsCheck(char* searchWord, int size, char** Grid, int gridRows, int gridCols, int*& position)
{
	for (int row = 0; row < gridRows; row++)
	{
		for (int col = 0; col < gridCols; col++)
		{
			if (searchWord[0] == Grid[row][col])    //first alphabet check
			{
				//horizontally backwards
				int wordIndex = 0, gridIndex = col, count = 0;
				while (gridIndex >= 0)
				{
					if (searchWord[wordIndex] == Grid[row][gridIndex])
					{
						count++;
						wordIndex++;
						gridIndex--;
					}
					else
						gridIndex = -1;  //to end the loop
				}

				if (count == size)
				{
					*(position) = row;
					*(position + 1) = col;
					*(position + 2) = row;
					*(position + 3) = col - (size - 1);
					return true;
				}
			}
		}
	}
	return false;
}

//this function checks if the word is present vertically downwards in the grid
bool VerticallyDownwardsCheck(char* searchWord, int size, char** Grid, int gridRows, int gridCols, int*& position)
{

	for (int col = 0; col < gridCols; col++)
	{
		for (int row = 0; row < gridRows; row++)
		{
			if (searchWord[0] == Grid[row][col])   //first alphabet check
			{
				//check vertically downwards
				int wordIndex = 0, gridIndex = row, count = 0;

				while (gridIndex < gridCols)
				{
					if (searchWord[wordIndex] == Grid[gridIndex][col])
					{
						count++;
						wordIndex++;
						gridIndex++;
					}
					else
						gridIndex += gridCols;  //to end the loop
				}

				if (count == size)
				{
					*(position) = row;
					*(position + 1) = col;
					*(position + 2) = row + (size - 1);
					*(position + 3) = col;

					return true;
				}
			}
		}
	}
	return false;
}

//this function checks if the word is present vertically upwards in the grid
bool VerticallyUpwardsCheck(char* searchWord, int size, char** Grid, int gridRows, int gridCols, int*& position)
{
	for (int col = 0; col < gridCols; col++)
	{
		for (int row = 0; row < gridRows; row++)
		{
			if (searchWord[0] == Grid[row][col])   //first alphabet check
			{
				//check vertically upwards
				int wordIndex = 0, gridIndex = row, count = 0;
				while (gridIndex >= 0)
				{
					if (searchWord[wordIndex] == Grid[gridIndex][col])
					{
						count++;
						wordIndex++;
						gridIndex--;
					}
					else
						gridIndex = -1;  //to end the loop
				}

				if (count == size)
				{
					*(position) = row;
					*(position + 1) = col;
					*(position + 2) = row - (size - 1);
					*(position + 3) = col;
					return true;
				}
			}
		}
	}
	return false;
}

//this function checks if the word is present diagonally left to right downwards in the grid
bool DiagonalOneCheck(char* searchWord, int size, char** Grid, int gridRows, int gridCols, int*& position)
{
	for (int row = 0; row < gridRows; row++)
	{
		for (int col = 0; col < gridCols; col++)
		{
			if (searchWord[0] == Grid[row][col])  //first alphabet check
			{
				int wordIndex = 0, rowIndex = row, colIndex = col, count = 0;

				//left to right downwards
				while (rowIndex < gridRows && colIndex < gridCols)
				{
					if (searchWord[wordIndex] == Grid[rowIndex][colIndex])
					{
						count++;  //to compare with word length
						wordIndex++;
						rowIndex++;
						colIndex++;
					}
					else
					{
						//to end the loop
						rowIndex += gridRows;
						colIndex += gridCols;
					}
				}

				if (count == size)
				{
					*(position) = row;
					*(position + 1) = col;
					*(position + 2) = row + (size - 1);
					*(position + 3) = col + (size - 1);
					return true;
				}

			}
		}
	}
	return false;
}

//this function checks if the word is present diagonally right to left upwards in the grid
bool DiagonalTwoCheck(char* searchWord, int size, char** Grid, int gridRows, int gridCols, int*& position)
{
	for (int row = 0; row < gridRows; row++)
	{
		for (int col = 0; col < gridCols; col++)
		{
			if (searchWord[0] == Grid[row][col])  //first alphabet check
			{
				int wordIndex = 0, rowIndex = row, colIndex = col, count = 0;

				//right to left upwards
				while (rowIndex >= 0 && colIndex >= 0)
				{
					if (searchWord[wordIndex] == Grid[rowIndex][colIndex])
					{
						count++;
						wordIndex++;
						rowIndex--;
						colIndex--;
					}
					else
					{
						//to end the loop
						rowIndex = -1;
						colIndex = -1;
					}
				}

				if (count == size)
				{
					*(position) = row;
					*(position + 1) = col;
					*(position + 2) = row - (size - 1);
					*(position + 3) = col - (size - 1);
					return true;
				}
			}
		}
	}
	return false;
}

//this function checks if the word is present diagonally left to right upwards in the grid
bool DiagonalThreeCheck(char* searchWord, int size, char** Grid, int gridRows, int gridCols, int*& position)
{
	for (int row = 0; row < gridRows; row++)
	{
		for (int col = 0; col < gridCols; col++)
		{
			if (searchWord[0] == Grid[row][col])  //first alphabet check
			{
				int wordIndex = 0, rowIndex = row, colIndex = col, count = 0;

				//left to right upwards
				while (rowIndex >= 0 && colIndex < gridCols)
				{
					if (searchWord[wordIndex] == Grid[rowIndex][colIndex])
					{
						count++;
						wordIndex++;
						rowIndex--;
						colIndex++;
					}
					else
					{
						//to end the loop
						rowIndex = -1;
						colIndex += gridCols;
					}
				}

				if (count == size)
				{
					*(position) = row;
					*(position + 1) = col;
					*(position + 2) = row - (size - 1);
					*(position + 3) = col + (size - 1);
					return true;
				}
			}
		}
	}
	return false;
}

//this function checks if the word is present diagonally right to left downwards in the grid
bool DiagonalFourCheck(char* searchWord, int size, char** Grid, int gridRows, int gridCols, int*& position)
{
	for (int row = 0; row < gridRows; row++)
	{
		for (int col = 0; col < gridCols; col++)
		{
			if (searchWord[0] == Grid[row][col])  //first alphabet check
			{
				int wordIndex = 0, rowIndex = row, colIndex = col, count = 0;

				//right to left downwards
				while (rowIndex < gridRows && colIndex >= 0)
				{
					if (searchWord[wordIndex] == Grid[rowIndex][colIndex])
					{
						count++;
						wordIndex++;
						rowIndex++;
						colIndex--;
					}
					else
					{
						//to end the loop
						rowIndex += gridRows;
						colIndex = -1;
					}
				}

				if (count == size)
				{
					*(position) = row;
					*(position + 1) = col;
					*(position + 2) = row + (size - 1);
					*(position + 3) = row - (size - 1);
					return true;
				}
			}
		}
	}
	return false;
}

//this function stores results in an Output.txt
void OutputFile(ofstream& results, int* position)
{
	int i = 0;
	if (*position != -1)
	{
		results << "{";
		while (i < 4)  //4 coordinate integers
		{
			results << *(position + i);
			i++;

			if (i == 1 || i == 3)
				results << ",";

			if (i == 2)
				results << "} {";
		}
		results << "}" << endl;
	}
	else
		results << "not found";
}

//this function stores results in an Output.txt
void OutputConsole(int* position)
{
	cout << endl << endl << "The obtained coordinates for the sent words : ";

	int i = 0;
	if (*position != -1)
	{
		cout << "{ ";
		while (i < 4)  //4 coordinate integers
		{
			cout << *(position + i);
			i++;

			if (i == 1 || i == 3)
				cout << " , ";

			if (i == 2)
				cout << " } { ";
		}
		cout << " }" << endl << endl;
	}
	else
		cout << "not found" << endl;
}

//generates and places a random alphabet on empty places
void GenerateRandomChar(char** &grid , int gridRows , int gridCols)
{
	for (int rows = 0; rows < gridRows; rows++)
	{
		for(int cols = 0; cols < gridCols; cols++)
		{
			int randomChar = (rand() % 26 - 32); 

			char rC = 'a' + randomChar;

			if (grid[rows][cols] == '-')                              
			{
				grid[rows][cols] = rC;
			}
		}
	}
}

//displays the 2D array
void PrintGrid(ofstream& finalGrid , char** grid , int size)
{
	char** endRows = grid + size;
	for (char** index = grid; index < endRows; index++)
	{ 
		for (char* j = *index ; j < *index + size; j++)
			finalGrid << *j << "    ";

		finalGrid << endl;
	}
	finalGrid << endl << endl;
}

//this function checks if the entered input/output files are correct or not
bool CorrectExtension (string fileName)
{
	int length;

	length = fileName.length();

	if (fileName.substr(length - 4, length) != ".txt")
	{
		cout<< "Invalid file type! Please enter a txt file";
		return false;
	}
	else
		return true;	
}

//swapping function
void Swap(char ** gridRow , int col)
{
	if (strlen(gridRow[col]) < strlen(gridRow[col + 1]))
	{
		
		swap(gridRow[col], gridRow[col + 1]);
	}
}

//this function sorts the words list that will be used to create the grid
void Sort(char** grid, int totalWords)
{
	for (int i = 1; i < totalWords; i++)
	{
		for (int j = 0; j < totalWords- i; j++)
		{
			
			Swap(grid, j);
		}
	}
}

//deallocates a 2D array
void deallocateArray (char** createdArray , const int& rows)
{
	for (int index = 0; index < rows; index++)
	{
		delete [] createdArray[index];
	}
	delete [] createdArray;
}

void CreatFunction (char** words , char** &grid , int index , int totalWords , int gridRows , int gridCols)
{
	bool HF_placing = false , HB_placing = false, VD_placing = false , VU_placing = false, 
		 D1_placing = false , D2_placing = false, D3_placing = false, D4_placing = false; 

	int size = 0;
	char* searchWord = 0;

	if (index == 0)
	{
		size = strlen(*(words)); //size to allocate memory accordingly and check condition

		searchWord = new char[size + 1];
		strcpy_s(searchWord, size + 1, *(words));  //to deep copy word from 2D to 1D array

		HF_placing = HorizontallyForwardPlacing (grid , searchWord , gridRows , gridCols);

		//deallocate dynamic 1D array
		delete [] searchWord;

		index++;
	}
	
	if (index == 1)
	{
		for (int index = 1; index < totalWords; index++)
		{
			size = strlen(*(words + index)); //size to allocate memory accordingly and check condition

			searchWord = new char[size + 1];
			strcpy_s(searchWord, size + 1, *(words + index));  //to deep copy word from 2D to 1D array

			bool flag = false;
			int check = 0;
			while (flag == false ) //&& check < 8)
			{
				int placing = rand() % 8 + 1;
			
				if  (placing == 8)               
				{	
					HF_placing = HorizontallyForwardPlacing (grid , searchWord ,  gridRows, gridCols );
					if (HF_placing == true)
						flag = true;
				}
				if  (placing == 7)  
				{
					HB_placing = HorizontallyBackwardsPlacing (grid , searchWord ,  gridRows , gridCols);
					if (HB_placing == true)
						flag = true;
				}
				if  (placing == 6)                    
				{
					VD_placing = VerticallyDownwardsPlacing (grid , searchWord ,  gridRows , gridCols);
					if (VD_placing == true)
						flag = true;
				}
				if  (placing == 5)                    
				{
					VU_placing = VerticallyUpwardsPlacing (grid , searchWord ,  gridRows , gridCols);
					if (VU_placing == true)
						flag = true;
				}

				if  (placing == 3)                    
				{
					D1_placing = DiagonalOnePlacing (grid , searchWord ,  gridRows , gridCols);
					if (D1_placing == true)
						flag = true;
				}

				if  (placing == 4)                    
				{
					D2_placing = DiagonalTwoPlacing (grid , searchWord ,  gridRows , gridCols);
					if (D2_placing == true)
						flag = true;
				}

				if  (placing == 2)    
				{
					D3_placing = DiagonalThreePlacing (grid , searchWord ,  gridRows , gridCols);
					if (D3_placing == true)
						flag = true;
				}
				if  (placing == 1)                    
				{
					D4_placing = DiagonalFourPlacing (grid , searchWord ,  gridRows , gridCols);
					if (D4_placing == true)
						flag = true;
				}
			}

			//deallocate dynamic 1D array
			delete [] searchWord;
		}
	}
}

//this function searchs each word in the grid and return its coordinates in output file as well as console
void SearchFunction (ofstream& results, char** words, char** Grid, int& gridRows, int& gridCols, int* position , int totalWords)
{
	bool HF_placing = false , HB_placing = false, VD_placing = false , VU_placing = false, 
		D1_placing = false , D2_placing = false, D3_placing = false, D4_placing = false;

	char* searchWord = 0;
	int size;

	for (int index = 0; index < totalWords; index++)
	{
		size = strlen(*(words + index));  //size to allocate memory accordingly
		searchWord = new char[size + 1];
		strcpy_s(searchWord, size + 1, *(words + index));  //to deep copy word from 2D to 1D array	

		HF_placing = HorizonallyForwardCheck(searchWord, size, Grid, gridRows, gridCols, position);

		if (HF_placing == true)
		{
			OutputFile(results, position);
			OutputConsole(position);
		}
		else
		{
			HB_placing = HorizonallyBackwardsCheck(searchWord, size, Grid, gridRows, gridCols, position);

			if (HB_placing == true)
			{
				OutputFile(results, position);
				OutputConsole(position);
			}
			else
			{
				VD_placing = VerticallyDownwardsCheck(searchWord, size, Grid, gridRows, gridCols, position);

				if (VD_placing == true)
				{
					OutputFile(results, position);
					OutputConsole(position);
				}
				else
				{

					VU_placing = VerticallyUpwardsCheck(searchWord, size, Grid, gridRows, gridCols, position);

					if (VU_placing == true)
					{
						OutputFile(results, position);
						OutputConsole(position);
					}
					else
					{
						D1_placing = DiagonalOneCheck(searchWord, size, Grid, gridRows, gridCols, position);

						if (D1_placing == true)
						{
							OutputFile(results, position);
							OutputConsole(position);
						}
						else
						{
							D2_placing = DiagonalTwoCheck(searchWord, size, Grid, gridRows, gridCols, position);

							if (D2_placing == true)
							{
								OutputFile(results, position);
								OutputConsole(position);
							}
							else
							{
								D3_placing = DiagonalThreeCheck(searchWord, size, Grid, gridRows, gridCols, position);

								if (D3_placing == true)
								{
									OutputFile(results, position);
									OutputConsole(position);
								}
								else
								{
									D4_placing = DiagonalFourCheck(searchWord, size, Grid, gridRows, gridCols, position);

									if (D4_placing == true)
									{
										OutputFile(results, position);
										OutputConsole(position);
									}
									else
									{
										*position = -1; //to validate check in OutputFile
										OutputFile(results, position);
										OutputConsole(position);
									}
								}
							}
						}
					}
				}
			}
		}
		//dealloacte search word
		delete[] searchWord;
	}
}

int main()
{
	int gridRows = 0 , gridCols = 0 , largestWord = 0 , totalWords = 0;
	int length , size;

	char option;
	char** grid = 0;
	char** words = 0;
	char* searchWord = 0;

	string inputFile , inputFile2 ,outputFile;

	cout << endl << "-----------------------------Welcome to the Word Search Puzzle zone--------------------------- " << endl 
		<< endl << "Press the first capital letter of your desired choice " << endl << endl;
	cout << "1. Create Grid " << endl;
	cout << "2. Search Words from Grid " << endl;
	cout << "3. Quit " << endl << endl;

	cin >> option;

	if (option == 'C')
	{
		int index;
		cout<<"Enter name of the input file containing words:  ";
		cin >> inputFile;
		if (CorrectExtension (inputFile) == false)
		{
			cout<< "Invalid file type! Please enter a txt file. Please try again";
			return 0;
		}
		ifstream wordsData (inputFile.c_str());

		cout<<"Enter name of the file you want to save your grid in:  ";
		cin >> outputFile;
		if (CorrectExtension (outputFile) == false)
		{
			cout<< "Invalid file type! Please enter a txt file. Please try again";
			return 0;
		}
		ofstream gridGenerated (outputFile.c_str());

		cout << endl << endl;
		cout << "Total words in the file for creating the grid :  ";
		cin  >> totalWords;
		cout << endl;
		cout << "How many rows should be there in the grid : ";
		cin  >> gridRows;
		cout << endl;
		cout << "How many columns should be there in the grid : ";
		cin  >> gridCols;
		cout << endl;

		words = ReadWordsListFromFileToCreateGrid(wordsData , largestWord , totalWords);  //stores all the words to be added
		Sort (words , totalWords);

		if (largestWord > gridRows && largestWord > gridCols)
		{
			cout << "Grid cannot be formed, press any key to return to the home screen " << endl;
		}
		else
		{
			grid = SearchGrid(grid , gridRows , gridCols);  //stores the initial grid

			index = 0;
			CreatFunction (words , grid , index , totalWords , gridRows , gridCols);

			
			GenerateRandomChar(grid , gridRows, gridCols);
			PrintGrid(gridGenerated , grid , gridRows);
			cout << "Grid successfully saved in " << outputFile << " file" << endl;

			deallocateArray(grid , gridRows);
			deallocateArray(words , totalWords);
		}
	}

	if (option == 'S')
	{
		int* position = new int[4]; //to store coordinates
		char** Grid = 0;

		cout<<"Enter name of the input file containing data:  ";
		cin >> inputFile;
		if (CorrectExtension (inputFile) == false)
		{
			cout<< "Invalid file type! Please enter a txt file. Please try again";
			return 0;
		}
		ifstream data (inputFile.c_str());
		words = ReadDataFromFileToSearchWords(data, gridRows, gridCols, totalWords);

		cout<<"Enter name of the input file containing grid:  ";
		cin >> inputFile2;
		if ( CorrectExtension (inputFile2) == false)
		{
			cout<< "Invalid file type! Please enter a txt file. Please try again";
			return 0;
		}
		ifstream gridData (inputFile2.c_str());
		Grid = InputGrid(gridData, gridRows , gridCols);

		cout<<"Enter name of the file where you want to save the results : ";
		cin >> outputFile;
		if (CorrectExtension (outputFile) == false)
		{
			cout<< "Invalid file type! Please enter a txt file. Please try again";
			return 0;
		}
		ofstream results (outputFile.c_str());

		SearchFunction (results, words,  Grid, gridRows, gridCols, position, totalWords);

		cout << "Results available in Output file" << endl;

			//deallocate position: 1D dynamic integer array
			delete[] position;
			deallocateArray (Grid , gridRows);
			deallocateArray (words, totalWords);
		}

		if (option == 'Q')
		{
			cout << "the program exits " << endl;
		}
		system("pause");
		return 0;
	}