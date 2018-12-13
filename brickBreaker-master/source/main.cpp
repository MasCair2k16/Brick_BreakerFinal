/////////////////////////
// Brick Breaker
// 11/29/18
// by mason caird & benjamin greenwood
// CS 172 final
/////////////////////////

#include"gameObjects.h"
#include"fstream"
using namespace std;

bool collide(COORD ballPos, COORD blockPos)   //compares two objects
{
    for (int i = 0; i < 7; i++)   //checks to see if ball collides with any of the block's positions
    {
        if (ballPos.X == blockPos.X && ballPos.Y == blockPos.Y)     // If they collide return true
        {
            return true;
        }
        blockPos.X++;
    }
    return false;
} 
bool gameOver(vector<GameObject*> go, int& blocksHit)     // returns true if all 10 stillBoxes have been hit
{
    for (int i = 0; i < go.size(); i++)
    {
        if (typeid(go[i]) == typeid(stillBox))  //checks to see if the gameobject is a still box
        {
            dynamic_cast<stillBox *>(go[i]);
            if ( dynamic_cast<stillBox *>(go[i])->getShape() == "       ")  // counts up bocksHit if still box has been hit
            {
                blocksHit++;
            }
        }
    }
    if(blocksHit == 10)
    {
        return true;
    }
    return false;
}

void overlap(vector<GameObject*> &go ) { //Reassures all stillboxes are not overlapping // Bugs to be fixed
    for (int i = 3; i<go.size(); i++){   // the index in our vector of stillboxes starts at 2 and we use i-1 so our for loop starts at 3
        for (int j = 0; j < 7; j++)     //makes sure it doesn't overlap any of the positions
        {
            while (dynamic_cast<stillBox *>(go[i-1])->getX()+j == dynamic_cast<stillBox *>(go[i])->getX() ||
                dynamic_cast<stillBox *>(go[i-1])->getX() == dynamic_cast<stillBox *>(go[i])->getX()+j)     //makes sure the boxes don't ovewrlap
            {
                dynamic_cast<stillBox *>(go[i-1])->newX(); // makes the stillbox in a new place
                dynamic_cast<stillBox *>(go[i-1])->newY();

            }
        }
    } 
}

int outputtxt(int blocksHit) { // This will output how many blocks were hit on a txt file.

    ofstream output; // File stream object and will be used to open the text file
    output.open("BrickBreakerLog.txt", ios::app);
    
    if(output.fail()) { // if the files fails to open, this shows
        cout << "Oops, can't open file" << endl;
        return 0;
    }
    output << "Bricks hit: " << blocksHit << endl;
    output.close();
    
    return 0;
}


int main() 
{
    srand(unsigned(time(NULL))); // Ranadom generator
    vector<GameObject*> gObjects; // pointer variable vector
    int brickshit = 0; // variable is used for outputTxt function
    remove("BrickBreakerLog.txt"); // Removes the old txt file to star new for each game.
    
    // Get handles to STDIN and STDOUT. 
    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    if (hStdin == INVALID_HANDLE_VALUE || hStdout == INVALID_HANDLE_VALUE) 
    {
        MessageBox(NULL, TEXT("GetStdHandle"), TEXT("Console Error"), 
            MB_OK);
        return 1;
    }
    
    gObjects.push_back(new userBox()); // placed a user box
    gObjects.push_back(new Ball('O')); // placed ball with a character
    for (int i = 0; i < 10; i++) // placed 11 random stillboxes.
    {
        gObjects.push_back(new stillBox());
    }
    overlap(gObjects); // Checks if the stillboxes are overlapping each other
    int key = 0;

    while (gameOver(gObjects, brickshit) != true) //makes sure the game isn't over 
    {  
        for (int i = 0; i < gObjects.size(); i++)       // updates game objects
        {   
            for (int j = i+1; j < gObjects.size(); j++ )    // checks for collisons
            {
                if (collide(gObjects[i]->get_pos(), gObjects[j]->get_pos()) == true)    // if the two objects we are looking at collide
                {   
                    brickshit++; // Increments

                    outputtxt(brickshit); // prints to file
                    if (typeid(*gObjects[i]) == typeid(stillBox) && typeid(*gObjects[j]) == typeid(Ball) || //checks to see if objects were stillbox & ball
                    typeid(*gObjects[j]) == typeid(stillBox) && typeid(*gObjects[i]) == typeid(Ball))    
                    {  
                        if (typeid(*gObjects[i]) == typeid(stillBox)) // finds which one was the still box and calls "hit"
                        {
                            dynamic_cast<stillBox *>(gObjects[i])->hit();
                        }
                        else
                        {
                            dynamic_cast<stillBox *>(gObjects[j])->hit();
                        }
                    }

                    if (typeid(*gObjects[i]) == typeid(userBox) && typeid(*gObjects[j]) == typeid(Ball) || //checks to see if objects were userBox & ball
                    typeid(*gObjects[j]) == typeid(userBox) && typeid(*gObjects[i]) == typeid(Ball))
                    {
                        if (typeid(*gObjects[i]) == typeid(Ball)) // finds which one was the ball and calls "flipVy"
                        {
                            dynamic_cast<Ball*>(gObjects[i])->flipVy();
                        }
                        else
                        {
                            dynamic_cast<Ball *>(gObjects[j])->flipVy();
                        }
                    }
                }
            }
        dynamic_cast<userBox *>(gObjects[0])->move(key);    // i know the first game object is the userbox, so call the special move function
        gObjects[i]->draw(); // These are called to draw and move the game object after each while loop.
        gObjects[i]->move();
        }
		// not our code. https://www.geeksforgeeks.org/kbhit-c-language/
 		for ( int x=0; x < 25; x++ ) {  
		   /* Use _getch to throw key away. */
			if ( _kbhit() ) {
				key =  _getch();
			}
			Sleep(5);
		}
    system("cls");
    ShowConsoleCursor(false);
    }
    ShowConsoleCursor(true);
    brickshit = 0;
    return 0;
}

