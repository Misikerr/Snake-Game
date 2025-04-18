

---

# 🐍 Snake Game - Console Edition (C++){}

A classic Snake game written in C++ with fun modern features like colorful fruits, timed special food, game timer, high score saving, and obstacles. Play in the console, avoid obstacles, and try to beat your best time and high score!

---

## 🎮 Features

 - ✅ Classic Snake gameplay in the console 
- ⏱️ Real-time game timer  
- 🥇 High Score and Best Time saving to `highscore.txt`  
- 🟦 Colorful fruit and special food
- 💎 Special food appears every 10 points and lasts 4 seconds  
- ⛔ Randomly generated obstacles for an extra challenge  
- ⏸️ Pause & Resume functionality (`P` key)  
- 💀 Game over on collision with tail or obstacles

---

## 🛠️ Requirements

- Windows OS  
- C++ compiler (e.g., g++, MSVC)  
- Console must support Windows-specific headers like `<windows.h>` and `<conio.h>`

---

## 🔧 How to Compile

Use a C++ compiler that supports Windows headers:

### Using g++ (MinGW):
```bash
g++ -o snake_game snake_game.cpp
```

### Using Visual Studio (Developer Command Prompt):
```cmd
cl snake_game.cpp
```

---

## ▶️ How to Play

- Arrow Keys → Move the snake  
- `P` → Pause or Resume the game  
- `X` → Exit the game  
- Eat fruit (`#`) to grow your tail and gain points  
- Eat special food (`$`) quickly before it disappears  
- Avoid obstacles (`X`) and your own tail!

---

## 💾 High Score Saving

- The game automatically saves your highest score and best time to a file named `highscore.txt`.
- The file will be created in the same folder if it doesn't exist.

---

## 🖼️ Console Display Example

```
                            ******************************
                            *                            *
                            *        O              #    *
                            *        o                   *
                            *                            *
                            *                            *
                            *                            *
                            *                            *
                            *                            *
                            ******************************
                            Time: 0:24
                            Best Time: 2:15
                            Score: 2
                            High Score: 15
                            Press P to Pause
```

---

## 🧠 Future Improvements

- Sound effects for fruit and special food  
- Different difficulty levels  
- Save/Load game state  
- Leaderboard for multiple users

---

## 💌 About the Developer
This game was built with passion, patience, and a lot of love for learning and coding.
Crafted by Misiker(Rio) — a curious developer from Ethiopia, dreaming big and building one line of code at a time. 💻💖

---

## 📜 License

This project is open-source and free to use for personal or educational purposes.

---
