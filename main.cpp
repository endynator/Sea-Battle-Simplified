#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>  
#include <chrono>  
#include <SFML/Audio.hpp>

using namespace std;

const int BOARD_SIZE = 5;

const string GREEN_COLOR = "\033[32m";
const string RESET_COLOR = "\033[0m";
const string RED_COLOR = "\033[31m";

void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    cout << "   A B C D E" << endl;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        cout << i + 1 << " |";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 'B') {
                cout << GREEN_COLOR << board[i][j] << RESET_COLOR << "|"; 
            }
            else if (board[i][j] == 'X') {
                cout << RED_COLOR << board[i][j] << RESET_COLOR << "|";
            }
            else {
                cout << board[i][j] << "|";
            }
        }
        cout << endl;
    }
    cout << endl;
}


bool isValidPlacement(char board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[y][x] != 'B');
}

int main() {
    sf::SoundBuffer buffer1;
    sf::SoundBuffer buffer2;

    sf::Sound hit, miss;
    sf::Music music;

    if (!buffer1.loadFromFile("hit.OGG")) {
        cout << "Unable to find file1.";
    }
    if (!buffer2.loadFromFile("miss.OGG")) {
        cout << "Unable to find file2.";
    }
    if (!music.openFromFile("music.WAV")) {
        cout << "Unable to find file3.";
    }

    hit.setBuffer(buffer1);
    miss.setBuffer(buffer2);
    
    music.setVolume(25); hit.setVolume(50); miss.setVolume(50);
    music.setLoop(true);
    music.play();

    char gameBoard[BOARD_SIZE][BOARD_SIZE];
    int NUM_BOATS = 3;
    srand(time(nullptr));

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            gameBoard[i][j] = '.';
        }
    }

    cout << "Welcome to the Boat Game!" << endl;
    cout << "You need to place three boats on the 5x5 board." << endl;

    for (int i = 0; i < NUM_BOATS; ++i) {
        printBoard(gameBoard);
        cout << "Place boat " << i + 1 << " (e.g., B3): ";
        string input;
        do {
            cin >> input;
            int x = input[0] - 'A';
            int y = input[1] - '1';
            if (isValidPlacement(gameBoard, x, y)) {
                gameBoard[y][x] = 'B';
                break;
            }
            else {
                cout << "Invalid placement. Try again: ";
            }
        } while (true);
    }

    cout << "Player's board:" << endl;
    printBoard(gameBoard);

    int boatsSunk = 0;
    while (boatsSunk < NUM_BOATS) {
        int x, y;
        do {
            x = rand() % BOARD_SIZE;
            y = rand() % BOARD_SIZE;
        } while (gameBoard[y][x] == 'X' || gameBoard[y][x] == 'O');

        this_thread::sleep_for(chrono::seconds(2));
        system("cls");
        if (gameBoard[y][x] == 'B') {
            hit.play();
            cout << "Computer hit your boat at " << static_cast<char>('A' + x) << y + 1 << "!" << endl;
            gameBoard[y][x] = 'X';
            boatsSunk++;
        }
        else {
            miss.play();
            cout << "Computer missed at " << static_cast<char>('A' + x) << y + 1 << "." << endl;
            gameBoard[y][x] = 'O';
        }

        printBoard(gameBoard);
        this_thread::sleep_for(chrono::seconds(2));
    }

    cout << "All your boats are sunk. You lost the game." << endl;

    music.stop();
    return 0;
}
