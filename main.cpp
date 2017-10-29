#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <curses.h>



// global variables
bool lives = true;
bool dies = false;

using namespace std;


void FileToVector(vector<vector<bool> >& , string, vector<vector<bool> >&); //Takes a file and puts bool value to 2 vectors
void displayVector(vector<vector<bool> >&); //Prints the bool vector on screen
void dimensions(const vector<vector<bool> >&,size_t&, int&);  //gets the dimensions of the 2d vector
void nextGen(vector<vector<bool> >&, vector<vector<bool> >&,size_t,int); // determines who lives or dies for next generation
void determinelife(vector<vector<bool> >&,int,int,int);  //sees if the current block dies or lives
void updateGame(vector<vector<bool> >&, vector<vector<bool> >&); //makes the old vector = to the change vector in order to display
void VectorToFile(vector<vector<bool> >& v,const string& );
string option();


int main()
{
    
    size_t rows; // number of rows on 2dvector
    int colums; // number of colums on 2dvector
    
    vector<vector<bool> > v;   // Vector that holds the current change
    vector<vector<bool> > vChange; // Vcetor that would hold the next change
    string choice = option();
    FileToVector(v,choice,vChange); //reads file to 2 vectors
    dimensions(v,rows,colums);  //gets lenght and widht of vector
    displayVector(v);           //prints the original vector
    
    for(int i=0;i<1000;i++) {  //loops to the next gen until it stops
        
        nextGen(v,vChange,rows,colums);  //gets next generation
        updateGame(v,vChange);           // sets vChange = to v
        cout << "next: " << endl;
        displayVector(v);//prints v which was changed by vChange
        usleep(100000);
        VectorToFile(v,"test2");
        
    }
    

    return 0;
    
}


void FileToVector(vector<vector<bool> >& input, string fileName, vector<vector<bool> >& input2 )
{
    string inFileName = fileName;
    ifstream inFile;
    inFile.open(inFileName);
    
    if (inFile.is_open())
    {
        string line;
        int count = 0;
        while (inFile)
        {
            getline(inFile, line);
            //Checks and make sure that the line variable isnt an empty string
            if (line != "")
            {
                //Creates a temporary vector and increments the size counter of the global vector
                vector<bool> tempVector;
                input.push_back(tempVector);
                input2.push_back(tempVector); //making 2 vectors from file
                for (size_t i = 0, j = line.length(); i < j; i++)
                {
                    //Depending on the character it input a 0 or 1 into a vector
                    if (line[i] == 'O')
                    {
                        input[count].push_back(1);
                        input2[count].push_back(1);
                    }
                    else
                    {
                        input[count].push_back(0);
                        input2[count].push_back(0);
                    }
                }
                count++;
            }
        }
        inFile.close();
    }
    else
    {
        cout << "Can not open file: " << inFileName << endl;
    }
    
}
void displayVector(vector<vector<bool> >& v) { // prints vector that we pass as parameter
    for(int i=0; i<v.size();i++) {
        
        for(int j=0;j<v[i].size();j++) {
            if(v[i][j]==true) {
                cout << "O";   //if its alive it will print out an O
            }
            else {
                cout <<".";  //otherwise dead so .
            }
            
        }
        cout << endl;
    }
    
}


void dimensions(const vector<vector<bool> >& v ,size_t& rows, int& colums) {  //gets the dimensions of the file
    rows = v.size();   //# of rows is just the size of the 2d vector
    
    for(int i =0;i<v.size();i++) {
        for(int j=0;j<v[1].size();j++) {
            colums = j+1;        //number of colums is the size of the vector at an index and plus 1 due to the index starting at 0
        }
        
    }
}


void nextGen(vector<vector<bool> >& v , vector<vector<bool> >& vChange, size_t rows, int colums) {
    
    // run through all elements on the 2d vector
    
    for(int i=0; i<v.size(); i++) {
        for(int j=0; j<v[i].size(); j++) {
            //idea: first and last row will have different conditions then middle
            //
            int live =0;  //keeps count of how many live neigboors are for each cell
            // note: it resets to 0 for each element which is what we want
            
            if(i==0) {  //first row of vector = v[i]=0
                
                if(j==0) { //first colum of vector  = v[i][j] when i=0.j=0;
                    
                    //Checking neighboors
                    
                    if(v[i+1][j]==1) { //checks bellow
                        live++;
                    }
                    if(v[i][j+1]==1) { //checks to the right
                        live++;
                    }
                    if(v[i+1][j+1] == 1) { //checks bottom right
                        live++;
                    }
                    if(v[rows-1][j]== 1) { //checks top
                        live++;
                    }
                    if(v[rows-1][j+1] == 1) { //checks top right
                        //rows is the last i that we can go down to
                        live++;
                    }
                    if(v[i][colums-1]== 1) { //checks one to the left
                        //colums is the last j we can go down to
                        live++;
                    }
                    if(v[i+1][colums-1]==1) { //checks bottom left
                        live++;
                    }
                    if(v[rows-1][colums-1]== 1) { // checks top left
                        live++;
                    }
                }
                
                else if(j==colums-1) { // last colum of vector = v[i][j]; i=0,j=last colums
                    
                    if(v[i+1][j]==1) { //checks bellow
                        live++;
                    }
                    if(v[i][0]==1) { //checks to the right
                        live++;
                    }
                    if(v[i+1][0] == 1) { //checks bottom right
                        live++;
                    }
                    if(v[rows-1][j]== 1) { //checks top
                        live++;
                    }
                    if(v[rows-1][0] == 1) { //checks top right
                        live++;
                    }
                    if(v[i][j-1]== 1) { //checks one to the left
                        live++;
                    }
                    if(v[i+1][j-1]==1) { //checks bottom left
                        live++;
                    }
                    if(v[rows-1][j-1]== 1) { // checks top left
                        live++;
                    }
                    
                }
                else {  // in betweenm  = v[i][j]= i=0, j=between last and first
                    
                    if(v[i+1][j]== 1) {  //checks bellow
                        live++;
                    }
                    if(v[i][j+1]==1) { // checks to the right
                        live++;
                    }
                    if(v[i+1][j+1] == 1) {   // checks bottom right
                        live++;
                    }
                    if(v[rows-1][j]== 1) {  // checks top one
                        live++;
                    }
                    if(v[rows-1][j+1]==1) {
                        live++;           // checks top right
                    }
                    if(v[i][j-1] == 1) {
                        live++;                 // checks left one
                    }
                    if(v[i+1][j-1] == 1) {   // checks bottom left
                        live++;
                    }
                    if(v[rows-1][j-1]==1) {
                        live++;                    // checks top left
                    }
                    
                }
                determinelife(vChange,i,j,live); //after checking all neighbors
                //pass coordinates and how many live it has counted
                //note: after calling this function this cell changes its value or stays the same
            }
            //
            else if(i==rows-1) { //Last row of vector
                
                if(j==0) { //first colum of vector        = v[i][j] ; i=last colum, j=0
                    
                    if(v[0][j]==1) { //checks bellow
                        live++;
                    }
                    if(v[i][j+1]==1) { //checks to the right
                        live++;
                    }
                    if(v[0][j+1] == 1) { //checks bottom right
                        live++;
                    }
                    if(v[i-1][j]== 1) { //checks top
                        live++;
                    }
                    if(v[i-1][j+1] == 1) { //checks top right
                        live++;
                    }
                    if(v[i][colums-1]== 1) { //checks one to the left
                        live++;
                    }
                    if(v[0][colums-1]==1) { //checks bottom left
                        live++;
                    }
                    if(v[i-1][colums-1]== 1) { // checks top left
                        live++;
                    }
                    
                }
                else if(j==colums-1) { // last colum of vector   = v[i][j]; i=last row , j=last colum
                    
                    if(v[0][j]==1) { //checks bellow
                        live++;
                    }
                    if(v[i][0]==1) { //checks to the right
                        live++;
                    }
                    if(v[0][0] == 1) { //checks bottom right
                        live++;
                    }
                    if(v[i-1][j]== 1) { //checks top
                        live++;
                    }
                    if(v[i-1][0] == 1) { //checks top right
                        live++;
                    }
                    if(v[i][j-1]== 1) { //checks one to the left
                        live++;
                    }
                    if(v[0][j-1]==1) { //checks bottom left
                        live++;
                    }
                    if(v[i-1][j-1]== 1) { // checks top left
                        live++;
                    }
                    
                }
                else {  // in between          = v[i][j] ; i=last row, j=between last colum and first
                    
                    if(v[0][j]==1) { //checks bellow
                        live++;
                    }
                    if(v[i][j+1]==1) { //checks to the right
                        live++;
                    }
                    if(v[0][j+1] == 1) { //checks bottom right
                        live++;
                    }
                    if(v[i-1][j]== 1) { //checks top
                        live++;
                    }
                    if(v[i-1][j+1] == 1) { //checks top right
                        live++;
                    }
                    if(v[i][j-1]== 1) { //checks one to the left
                        live++;
                    }
                    if(v[0][j-1]==1) { //checks bottom left
                        live++;
                    }
                    if(v[i-1][j-1]== 1) { // checks top left
                        live++;
                    }
                }
                determinelife(vChange,i,j,live);
            }
            //
            else {  //its in between
                
                if(j==0) { //first colum of vector   = v[i][j] = i=between first row and last row, j=first colum
                    
                    if(v[i+1][j]==1) { //checks bellow
                        live++;
                    }
                    if(v[i][j+1]==1) { //checks to the right
                        live++;
                    }
                    if(v[i+1][j+1] == 1) { //checks bottom right
                        live++;
                    }
                    if(v[i-1][j]== 1) { //checks top
                        live++;
                    }
                    if(v[i-1][j+1] == 1) { //checks top right
                        live++;
                    }
                    if(v[i][colums-1]== 1) { //checks one to the left
                        live++;
                    }
                    if(v[i+1][colums-1]==1) { //checks bottom left
                        live++;
                    }
                    if(v[i-1][colums-1]== 1) { // checks top left
                        live++;
                    }
                    
                }
                else if(j==colums-1) { // last colum of vector   = v[i][j] = i=between first row and last row, j= last colum
                    
                    if(v[i+1][j]==1) { //checks bellow
                        live++;
                    }
                    if(v[i][0]==1) { //checks to the right
                        live++;
                    }
                    if(v[i+1][0] == 1) { //checks bottom right
                        live++;
                    }
                    if(v[i-1][j]== 1) { //checks top
                        live++;
                    }
                    if(v[i-1][0] == 1) { //checks top right
                        live++;
                    }
                    if(v[i][j-1]== 1) { //checks one to the left
                        live++;
                    }
                    if(v[i+1][j-1]==1) { //checks bottom left
                        live++;
                    }
                    if(v[i-1][j-1]== 1) { // checks top left
                        live++;
                    }
                }
                else {  // in between    = v[i][j] = i=between first row and last row, j= between last colum and first one
                    
                    if(v[i+1][j]==1) { //checks bellow
                        live++;
                    }
                    if(v[i][j+1]==1) { //checks to the right
                        live++;
                    }
                    if(v[i+1][j+1] == 1) { //checks bottom right
                        live++;
                    }
                    if(v[i-1][j]== 1) { //checks top
                        live++;
                    }
                    if(v[i-1][j+1] == 1) { //checks top right
                        live++;
                    }
                    if(v[i][j-1]== 1) { //checks one to the left
                        live++;
                    }
                    if(v[i+1][j-1]==1) { //checks bottom left
                        live++;
                    }
                    if(v[i-1][j-1]== 1) { // checks top left
                        live++;
                    }
                    
                }
                determinelife(vChange,i,j,live);
            }
            
            
        }
    }
    
}

void determinelife(vector<vector<bool> >&vChange ,int i,int j,int lives) { //call function for each element depending on location
    
    if(vChange[i][j]==false) {  //if the current block is dead
        if(lives == 3) {        // and there are live neigbors then it becomes alive
            vChange[i][j]=true;
        }                           // only way to come alive from dead is if there is 3 alive
    }
    
    if(vChange[i][j]==true) {  //if the current block is alive
        if(lives<2 || lives>3) {  // if there are less than 2 alives im becoming dead
            vChange[i][j]=false;
        }
    }
}

void updateGame(vector<vector<bool> >& v, vector<vector<bool> >& vChange) {  //takes the change vector and sets it equal to original v
    for(int i=0;i<v.size();i++) {                                           // this is necessary to print the right vector
        
        for(int j=0; j<v[i].size();j++) {
            v[i][j] = vChange[i][j];     //each element is equal to one another
        }
    }
}

void VectorToFile(vector<vector<bool> >& v,const string& fileName)
{
    ofstream outFile;
    string inFileName = fileName;
    outFile.open(inFileName);
    
    if (outFile.is_open())
    {
        for (size_t i = 0, j = v.size(); i < j; i++)
        {
            for (size_t k = 0, l = v[i].size(); k < l; k++)
            {
                if(v[i][k]==true){
                    outFile << 'O' << flush;
                }
                else{
                    outFile<<'.'<<flush;
                }
                // outFile << v[i][k] << flush;
                
                
            }
            outFile << endl;
        }
        cout << "File created: " << inFileName << endl;
    }
    else
    {
        cout << "Could not create file: " << inFileName << endl;
    }
}

string option() {
    int x;
    cout << "Choice: ";
    cin >> x;
    
    switch(x) {
        case 1:
            return "acorn-110x60.txt";
        case 2:
            return "diehard-80x60.txt";
        case 3:
            return "glider-40x20.txt";
        case 4:
            return "pentomino-80x60.txt";
        default:
            cout<<"try again \n";
            option();
    }
    return "";
}







