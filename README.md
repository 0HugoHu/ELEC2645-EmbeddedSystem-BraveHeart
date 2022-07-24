# ELEC2645-EmbeddedSystem-BraveHeart
An embedded system video game made with ARM LPC1768 board.

# Coursework Requirements
On completion of this unit, students should be able to:
- Write, compile, run and de-bug complex C++ programs using appropriate software engineering techniques.
- Implement advanced embedded software techniques on a microcontroller.
- Demonstrate initiative, independent learning, creativity, problem-solving, time management and project management skills.
- Present their work in an effective manner.

# Introduction

In this project, we were required to build an embedded system video game.

![](https://s1.ax1x.com/2022/07/24/jjpaLR.png)


# Preview
![](https://s1.ax1x.com/2022/07/24/jjpyWD.png)

🔱 **Game Flow:**

0. The prologue of the adventure is about to be unveiled…
“Where am I? Help me get out of here! Help!!!” (Fig.1)
![](https://s1.ax1x.com/2022/07/24/jjpcSe.png)
You are now in a dungeon, slay enemies and return to the ground!

1. First, you need to know how to move and fire (Fig.2).
![](https://s1.ax1x.com/2022/07/24/jjp2yd.png)


2. Next, complete 5 tutorials to get yourself ready! (Fig.3)
![](https://s1.ax1x.com/2022/07/24/jjpROA.png)

3. Now, you can show off your skills in real adventure! (Fig.4)
![](https://s1.ax1x.com/2022/07/24/jjpgQH.png)
All elements including the enemies, mazes, coins are randomly generated! You may find bosses, chests, and stores in this endless-level dungeon!
You can fire to enemies, use ultimate skills, buy something to boost your abilities! Different kinds of enemies may even attack player proactively.

4. Tired of adventure? Let’s have a rest and appreciate the records you have made in games! (Fig.5)
![](https://s1.ax1x.com/2022/07/24/jjpfeI.png)

🏁 **Conclusion:**

Never be afraid of failures, keep going man, you can always challenge yourself here!

# Project Highlights

🌟 **Shining Points**

**1. Random elements. [Technical]**
🎈 Inspirations: People get tired with fixed experience; random objects create fun.
📘 Engineering Principles: Stability. The code must ensure that the randomly generated items are reasonable, such as not placing gold coins on the wall, not placing the character's birth point and the enemy's birth point too close together.
- Maps: You never know what the next map is. There are totally 4 kinds (normal, chest, store, boss) and 10 maps, they are randomly distributed. (Fig.1-2)

|  |  |
|-|-|
|![](https://s1.ax1x.com/2022/07/24/jj9ek6.png) |![](https://s1.ax1x.com/2022/07/24/jj9KpD.png) |

- Enemies: Five kinds of enemies, each has its special abilities (Fig.3). The type, the number, the positions they generated are all random. Enemy has both HP and SHIELD; special enemies stop shooting fire balls unless their shields are broken.

![](https://s1.ax1x.com/2022/07/24/jj9VTx.png)

- Gold coins & birth point & success point: Randomly distributed on maps.

**2. Chest System  Innovative**

🎈 Inspirations: Hard work needs to be rewarded.

Generally, player has 15% chance to enter a chest room. Player gets awards by unlocking the chest. Player can view his ability panel. (Fig.4)

![](https://s1.ax1x.com/2022/07/24/jj9mtK.png)

**3. Store System Innovative**

🎈 Inspirations: Players should be able to upgrade their equipment to face more formidable enemies.

Player has 10% chance to enter a store. Player can buy ultimate skills (SP) to kill enemies in range immediately.
![](https://s1.ax1x.com/2022/07/24/jj9AmR.png)

**4. Tutorial System Technical**

🎈 Inspirations: It is difficult for players to get started with an unfamiliar game.

📘 Engineering Principles: Integrity & Maintainability. The tutorial module inherits directly from the game module, only modifying the parameters of each layer to make it meet the tutorial requirements. I used global variables to mark the tutorial progress so that after the player completes a certain tutorial, it automatically returns to the text prompt for the next tutorial without requiring the player to press any buttons.

Five step-by-step tutorials are designed where you will learn how to move to the target point, how to pick up gold coins, open treasure chests, how to shoot, and how to use the ultimate skill. (Fig.6-7)

![](https://s1.ax1x.com/2022/07/24/jj9YAP.png)
![](https://s1.ax1x.com/2022/07/24/jj9d1g.png)

**5. Achievements System  Innovative**

🎈 Inspirations: I love statistic data, I love to see what I have achieved in previous game.

File system is used, those records are stored in a txt file locally.

![](https://s1.ax1x.com/2022/07/24/jj90Xj.png)

**6. Others**

- Endless game levels, with difficulty increasing exponentially.
- Fantastic background music (Zelda) and fire & interact sound effects.
- Adjust the screen brightness to the light.
- ShiftReg LED and LPC1768-on-board LEDs are used to show player’s lives left, and ultimate skills left correspondingly. (Fig.9)
- Limit the game frame rate to 12 fps to prevent flickering when the lcd is refreshed too fast for complete rendering.
- The program is written based on fsm and modularity, which makes it possible to create tutorial levels by adding only tens of lines of code.

![](https://s1.ax1x.com/2022/07/24/jj980I.png)

# Project Reflection

📝 **Objectives**

💻 **1. Write, compile, run and de-bug complex C++ programs using appropriate software engineering techniques.**

- This task has been finished at least by the hard work I had done:

![](https://s1.ax1x.com/2022/07/24/jjCKU0.png)
![](https://s1.ax1x.com/2022/07/24/jjCM5V.png)


- Several engineering techniques were used including but not limit to: Object-oriented programming, fsm-based state control mechanism, modular page-based separation design, rational use of interrupts and timers, use of light sensors, LCD, remote sensing, LEDs, and four buttons.

⚙ **2. Implement advanced embedded software techniques on a microcontroller.**

Since there were many advanced techniques used, I only name their functions but not introduce how they were implemented (weekly diary covered some of them).

- Random number generator based on analog read-in, random generation algorithm for game elements.
- Very complex map initialization process.
- Collision event handling (entity and bullets).
- AI algorithm for enemy movement.
- Design of endless levels.
- Achievement system based on local file storage.

⌚ **3. Demonstrate initiative, independent learning, creativity, problem-solving, time management and project management skills.**

Everything I did followed the development timetable (see task list in every diary). This game was completed designed and created by me, not borrowed from other finished games (fsm framework and music excluded). Every initiative of features was demonstrated in Project Highlights part.

During the development, many problems occurred:

First, due to the limited RAM space of the LPC1768 processor, I could not create a matrix of 84*48 size to save the randomly generated maps, so I had to save as many maps as possible in the form of const in ROM and then call them randomly. Inspired by the idea of hierarchy, the map is only used as the bottom layer, and by drawing enemies and gold coins etc. on the top layer, I can freely change their positions without storing them in a large array to render at once.

The next is the generation of true random numbers (instead of using the build-in rand() function) and collision body detection, these two implementations are explained in detail in the diary and will not be expanded here.

The most difficult problems to solve include how to design the tutorial interface. The ideal tutorial should show a text, then the player enters the game and gives feedback at the end. But the tutorial and the game are two event states of the same level, which can't call each other, and I can't copy the game flow with thousands of lines of code again because of the tutorial.  My final thought was to design a global variable to add a judgment mechanism in all the different states of fsm, when the game is triggered by a tutorial, a specific level is shown and automatically returns to the main menu after it ends, and then to the next tutorial text screen without the user pressing any button.

Finally, a small question, because I limit the maximum frame rate of the game to 15fps (to prevent flicker caused by incomplete refresh due to too fast refresh), the background music does not play properly (the minimum period of note sounding and the chip each sleep start period are not synchronized). My solution is very simple: the frame rate is still limited, but the processor no longer sleeps.

🎉 **4. Present their work in an effective manner.**

----------

🎗 **Reflection**

Through this mini-project, I fully understood and applied what I learned in class about event mechanisms, interrupting and sleeping, and the use of joysticks and screens. I have initial experience in how to design and follow a plan to implement a large project step by step. I also fully realized the importance of some engineer guidelines.

1. Have a solid foundation.
Without a solid professional foundation, it would be difficult for me to use data structures such as vector, map, file system, etc. correctly, use pointers properly and avoid memory leaks.

2. Have a rich imagination.
To design a game, the most important thing is to create something fun and it is worth to playing. The player can fire to the enemy, but how cannot a dragon shoot fire balls to a player? Based on this logic imagination, I enabled enemies to attack.

3. Good project management.
You must must must follow the timetable exactly. Otherwise, you have to relinquish some brilliant ideas because you have no time to implement. A wise and reasonable plan is vital, you cannot expect yourself finish a very complex part in one week.
