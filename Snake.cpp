#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <fstream>

using namespace std;

// Add timer variables
DWORD gameStartTime;
DWORD totalPausedTime = 0;
DWORD pauseStartTime;
int bestTimeSeconds = 0;
bool timerStarted = false;

bool gameOver;
bool paused = false;
const int fieldWidth = 30, fieldHeight = 15;
const int fieldArea = fieldWidth * fieldHeight;
int headX, headY, fruitX, fruitY, specialFoodX, specialFoodY, score;
int highestScore;
enum eDirection { STOP, LEFT, RIGHT, UP, DOWN } direction;
int tailX[fieldArea], tailY[fieldArea];
int tailLength = 0;
int speedDelay = 75;
int fruitColor = 2;
int specialFoodColor = 2; // Separate color for special food
bool specialFoodActive = false;
DWORD specialFoodSpawnTime;
DWORD specialFoodPausedAtSpawn;
const int SPECIAL_FOOD_DURATION = 4; // 4 seconds

// Obstacle variables
const int MAX_OBSTACLES = 10;
int obstacleX[MAX_OBSTACLES], obstacleY[MAX_OBSTACLES];
int obstacleCount = 0;

void PlaceObstacles() {
    for(int i = 0; i < obstacleCount; i++) {
        bool validPosition;
        do {
            validPosition = true;
            obstacleX[i] = rand() % fieldWidth;
            obstacleY[i] = rand() % fieldHeight;
            
            // Avoid important positions
            if(obstacleX[i] == headX && obstacleY[i] == headY) validPosition = false;
            for(int j = 0; j < tailLength; j++) {
                if(obstacleX[i] == tailX[j] && obstacleY[i] == tailY[j]) {
                    validPosition = false;
                    break;
                }
            }
            if(obstacleX[i] == fruitX && obstacleY[i] == fruitY) validPosition = false;
            if(specialFoodActive && obstacleX[i] == specialFoodX && obstacleY[i] == specialFoodY) validPosition = false;
            for(int j = 0; j < i; j++) {
                if(obstacleX[i] == obstacleX[j] && obstacleY[i] == obstacleY[j]) {
                    validPosition = false;
                    break;
                }
            }
        } while(!validPosition);
    }
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void LoadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highestScore >> bestTimeSeconds;
        file.close();
    } else {
        highestScore = 0;
        bestTimeSeconds = 0;
    }
}

void SaveHighScore() {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highestScore << " " << bestTimeSeconds;
        file.close();
    }
}

void SetCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Setup() {
    timerStarted = false;
    gameStartTime = GetTickCount();
    totalPausedTime = 0;
    gameOver = false;
    direction = STOP;
    headX = fieldWidth / 2;
    headY = fieldHeight / 2;
    srand(time(NULL));
    fruitX = rand() % fieldWidth;
    fruitY = rand() % fieldHeight;
    specialFoodActive = false;
    score = 0;
    tailLength = 0;
    obstacleCount = 0;
}

void SetRandomFruitColor() {
    int colors[] = { 2, 4, 6, 9, 10, 12, 14 };
    fruitColor = colors[rand() % 7];
}

void PlaceFruit() {
    bool validPosition;
    do {
        validPosition = true;
        fruitX = rand() % fieldWidth;
        fruitY = rand() % fieldHeight;
        for (int i = 0; i < tailLength; ++i) {
            if (tailX[i] == fruitX && tailY[i] == fruitY) {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);
    SetRandomFruitColor();
}

void PlaceSpecialFood() {
    bool validPosition;
    do {
        validPosition = true;
        specialFoodX = rand() % fieldWidth;
        specialFoodY = rand() % fieldHeight;
        for (int i = 0; i < tailLength; ++i) {
            if (tailX[i] == specialFoodX && tailY[i] == specialFoodY) {
                validPosition = false;
                break;
            }
        }
        if (fruitX == specialFoodX && fruitY == specialFoodY)
            validPosition = false;
    } while (!validPosition);
    int colors[] = { 2, 4, 6, 9, 10, 12, 14 };
    specialFoodColor = colors[rand() % 7];
    specialFoodSpawnTime = GetTickCount();
    specialFoodPausedAtSpawn = totalPausedTime;
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwSize.X;
}

void Draw() {
    DWORD currentTime = GetTickCount();
    DWORD elapsedMillis = 0;
    int elapsedSeconds = 0;
    if (timerStarted) {
        elapsedMillis = currentTime - gameStartTime - totalPausedTime;
        elapsedSeconds = elapsedMillis / 1000;
    }
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;

    int bestMinutes = bestTimeSeconds / 60;
    int bestSeconds = bestTimeSeconds % 60;

    SetCursorPosition(0, 0);
    int consoleWidth = getConsoleWidth();
    int padding = (consoleWidth - fieldWidth - 2) / 2;

    SetColor(4);
    cout << setw(padding) << "" << string(fieldWidth + 2, '*') << endl;

    for (int i = 0; i < fieldHeight; ++i) {
        cout << setw(padding) << "" << "*";
        for (int j = 0; j < fieldWidth; ++j) {
            bool drawn = false;
            
            for(int k = 0; k < obstacleCount; k++) {
                if(obstacleX[k] == j && obstacleY[k] == i) {
                    SetColor(12);
                    cout << 'X';
                    drawn = true;
                    break;
                }
            }
            
            if(drawn) continue;
            
            if (i == fruitY && j == fruitX) {
                SetColor(fruitColor);
                cout << '#';
            } else if (specialFoodActive && i == specialFoodY && j == specialFoodX) {
                SetColor(specialFoodColor);
                cout << '$';
            } else if (i == headY && j == headX) {
                SetColor(11);
                cout << 'O';
            } else {
                bool tailSegment = false;
                for (int k = 0; k < tailLength; ++k) {
                    if (tailX[k] == j && tailY[k] == i) {
                        SetColor(10);
                        cout << 'o';
                        tailSegment = true;
                        break;
                    }
                }
                if (!tailSegment) cout << ' ';
            }
            SetColor(7);
        }
        SetColor(4);
        cout << "*" << endl;
    }

    cout << setw(padding) << "" << string(fieldWidth + 2, '*') << endl;
    SetColor(7);

    cout << setw(padding) << "" << "Time: " 
        << minutes << ":" << setw(2) << setfill('0') << seconds << setfill(' ') << endl;
    cout << setw(padding) << "" << "Best Time: "
        << bestMinutes << ":" << setw(2) << setfill('0') << bestSeconds << setfill(' ') << endl;
    cout << setw(padding) << "" << "Score: " << score << endl;
    cout << setw(padding) << "" << "High Score: " << highestScore << endl;
    cout << setw(padding) << "" << (paused ? "Press P to Resume" : "Press P to Pause") << endl;
}

void Input() {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0xE0 || ch == 0) {
            ch = _getch();
            switch (ch) {
                case 75: if (direction != RIGHT) direction = LEFT; break;
                case 77: if (direction != LEFT) direction = RIGHT; break;
                case 72: if (direction != DOWN) direction = UP; break;
                case 80: if (direction != UP) direction = DOWN; break;
            }
        } else {
            switch (ch) {
                case 'x': gameOver = true; break;
                case 'p':
                    paused = !paused;
                    if (paused) {
                        pauseStartTime = GetTickCount();
                    } else {
                        totalPausedTime += GetTickCount() - pauseStartTime;
                    }
                    break;
            }
        }
    }
}

void Logic() {
    if (paused) return;

    // Check if the snake has started moving and start the timer
    if (direction != STOP && !timerStarted) {
        gameStartTime = GetTickCount();
        timerStarted = true;
    }

    if (specialFoodActive) {
        DWORD currentTime = GetTickCount();
        DWORD currentTotalPaused = totalPausedTime;
        DWORD pausedDuringSpecialFood = currentTotalPaused - specialFoodPausedAtSpawn;
        DWORD elapsed = (currentTime - specialFoodSpawnTime) - pausedDuringSpecialFood;
        if (elapsed >= SPECIAL_FOOD_DURATION * 1000) {
            specialFoodActive = false;
        }
    }

    for (int i = tailLength - 1; i > 0; --i) {
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }
    if (tailLength > 0) {
        tailX[0] = headX;
        tailY[0] = headY;
    }

    switch (direction) {
        case LEFT: headX--; break;
        case RIGHT: headX++; break;
        case UP: headY--; break;
        case DOWN: headY++; break;
    }

    headX = (headX + fieldWidth) % fieldWidth;
    headY = (headY + fieldHeight) % fieldHeight;

    for (int i = 0; i < tailLength; ++i) {
        if (tailX[i] == headX && tailY[i] == headY)
            gameOver = true;
    }

    if (headX == fruitX && headY == fruitY) {
        score++;
        tailLength++;
        PlaceFruit();
        if (score % 10 == 0) {
            PlaceSpecialFood();
            specialFoodActive = true;
        }
        speedDelay = max(30, speedDelay - 3);
    }
    else if (specialFoodActive && headX == specialFoodX && headY == specialFoodY) {
        score += 4;
        tailLength += 4;
        specialFoodActive = false;
        speedDelay = max(30, speedDelay - 3);
    }

    int targetObstacles = min(score / 15, MAX_OBSTACLES);
    if (targetObstacles > obstacleCount) {
        obstacleCount = targetObstacles;
        PlaceObstacles();
        speedDelay = max(30, speedDelay - 15);
    }

    for(int i = 0; i < obstacleCount; i++) {
        if(headX == obstacleX[i] && headY == obstacleY[i]) {
            gameOver = true;
            return;
        }
    }
}

void GameOverScreen() {
    DWORD finalTime = 0;
    if (timerStarted) {
        finalTime = (GetTickCount() - gameStartTime - totalPausedTime) / 1000;
    }
    int finalMinutes = finalTime / 60;
    int finalSeconds = finalTime % 60;
    
    int bestMinutes = bestTimeSeconds / 60;
    int bestSeconds = bestTimeSeconds % 60;

    system("cls");
    SetCursorPosition(0, 0);
    int padding = (getConsoleWidth() - fieldWidth - 2) / 2;
    
    cout << setw(padding) << "" << "GAME OVER!\n";
    cout << setw(padding) << "" << "Score: " << score << endl;
    cout << setw(padding) << "" << "Your Time: " 
        << finalMinutes << ":" << setw(2) << setfill('0') << finalSeconds << setfill(' ') << endl;
    cout << setw(padding) << "" << "Best Time: "
        << bestMinutes << ":" << setw(2) << setfill('0') << bestSeconds << setfill(' ') << endl;

        bool newRecord = false;

        // Check if the new score EXCEEDS the previous high score
        if (score > highestScore) {
            highestScore = score;
            bestTimeSeconds = finalTime; // Always update time for new high score
            newRecord = true;
        }
        // Check if the new score MATCHES the high score but has a FASTER time
        else if (score == highestScore && finalTime < bestTimeSeconds) {
            bestTimeSeconds = finalTime; // Update time only if faster
            newRecord = true;
        }
    
        if (newRecord) {
            SaveHighScore(); // Save the new high score and best time
            cout << setw(padding) << "" << "New High Score!\n";
        }

    
    _getch();
}

int main() {
    LoadHighScore();
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(speedDelay);
    }
    GameOverScreen();
    return 0;
}