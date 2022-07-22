#include "Game.h"

/// @file Game.cpp

Game::~Game()
{
    clear();
}

void Game::clear()
{
    // Free allocated memory from bullets
    for (std::vector<Point*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
        delete* it;

    bullets.clear();

    // Free allocated memory from enemies
    for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
        delete* it;

    enemies.clear();

    // Set initial values for the player
    player.width = player.height = 5;

    player.vx = 0;
    player.vy = 0;

    player.dead = false;

    player.facingLeft = false;
}

void Game::init()
{
    if (currentLayer == 1) {
        Global::score = 0;
        paused = false;
        livesLeft = 3;
        memcpy(&mapProperty, &mapProperty1, sizeof(mapProperty1));
    }
    else
    {
        clear();
        switch (currentLayer)
        {
        case 2:
        {
            memcpy(&mapProperty, &mapProperty2, sizeof(mapProperty2));
            break;
        }
        case 3:

            break;
        case 4:

            break;
        }

    }
    const int seven_seg_array[] = {
        0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
    };
    shift.write(seven_seg_array[livesLeft]);

    respawnPlayer();

    spawnEnemy();
}



void Game::spawnEnemy()
{

    // Get random spawn point
    int r = rand() % 3;
    int x = mapProperty.enermyPosition[r][0];
    int y = mapProperty.enermyPosition[r][1];

    // Spawn random enemy
    int randPercent = (rand() % 100);
    Enemy::Type type;

    if (randPercent >= 40)      // 60% probability
        type = Enemy::JUMPER;
    else if (randPercent >= 15) // 25% probablitiy
        type = Enemy::SIMPLE;
    else                        // 15 % probability
        type = Enemy::RUNNER;

    // Create enemy
    Enemy* enemy = new Enemy(x, y, true, type);
    enemies.push_back(enemy);
}

// Functions
void Game::update(float dt)
{
    // Pause button input
    if (input->read(Input::ButtonC))
    {
        if (releasedBtnC)
        {
            paused = !paused;
            releasedBtnC = false;
        }
    }
    else
        releasedBtnC = true;

    // Skip the rest if paused
    if (paused) return;

    // if successfully reach the end
    if (player.x == mapProperty.succeedPosition[0] && player.y == mapProperty.succeedPosition[1])
    {
        currentLayer += 1;
        init();
    }

    // if respawn new enermy
    if ((rand() % 100) < mapProperty.spawnRate && !player.dead && enemies.size() <= mapProperty.maxEnermy)
        spawnEnemy();



    // Handle input, should be its own function
    switch (input->joystick->getDirection())
    {
    case LEFT:
        player.vx = -1;
        player.facingLeft = true;
        break;
    case UP_LEFT:
        player.vx = -1;
        player.vy = -1;
        player.facingLeft = true;
        break;
    case DOWN_LEFT:
        player.vx = -1;
        player.vy = 1;
        player.facingLeft = true;
        break;
    case RIGHT:
        player.vx = 1;
        player.facingLeft = false;
        break;
    case UP_RIGHT:
        player.vx = 1;
        player.vy = -1;
        player.facingLeft = false;
        break;
    case DOWN_RIGHT:
        player.vx = 1;
        player.vy = 1;
        player.facingLeft = false;
        break;
    case UP:
        player.vy = -1;
        player.facingLeft = false;
        break;
    case DOWN:
        player.vy = 1;
        player.facingLeft = false;
        break;
    case CENTER:
        player.vx = 0;
        player.vy = 0;
        break;
    }

    if (!player.dead)
        switch (currentLayer)
        {
        case 1:
            moveWithCollisionTest(&player, map1);
            break;
        case 2:
            moveWithCollisionTest(&player, map2);
            break;
        }

    else // move without testing collision agains the map
    {
        player.x += player.vx;
        player.y += 4;
    }

    moveEnemies();

    // Check if bullet should be fired
    if (input->read(Input::ButtonB) && releasedBtnB && !player.dead)
    {
        // Create a new bullet and give it initial values
        Point* bullet = new Point;
        bullet->x = (int)(player.x + (player.width / 2)); //(player.facingLeft) ? (player.x-1) : (player.x + player.width);
        bullet->y = player.y + 2;
        bullet->vx = (player.facingLeft) ? -4 : 4;
        bullet->vy = 0;

        bullets.push_back(bullet);
        releasedBtnB = false;

        // Play sound
        // sound->playNote(SFX::BULLET_FIRED);
    }
    else if (!input->read(Input::ButtonB))
        releasedBtnB = true;

    // Loop through bullets and move them + collision test
    for (std::vector<Point*>::iterator it = bullets.begin(); it != bullets.end();)
    {
        Point* bullet = *it;

        int x0; // left border of collision rect
        int x1; // right border of collision rect

        int oldX = bullet->x;
        int newX = bullet->x + bullet->vx;

        x0 = min(oldX, newX);
        x1 = max(oldX, newX);

        // Collision rect for bullet in this time step
        Rectangle bulletColRect(x0, bullet->y, (x1 - x0) + 1, 1);

        bool col = false;
        // Delete if outside screen
        switch (currentLayer)
        {
        case 1:
            if (newX < 0 || newX > WIDTH || bulletHitMap(bulletColRect, map1)) // if outside screen
                col = true;
            else
            {
                // loop through all enemies
                for (std::vector<Enemy*>::iterator ite = enemies.begin(); ite != enemies.end(); ++ite)
                {
                    Enemy* enemy = *ite;

                    // If bullet hits enemy
                    //if (!enemy->dead && bullet->x >= enemy->x && bullet->x <= enemy->getRight() && bullet->y >= enemy->y && bullet->y <= enemy->getBottom()) 

                    Rectangle enemyColRect(enemy->x, enemy->y, enemy->width, enemy->height); // collision rectangle for enemy

                    if (!enemy->dead && hitTestRect(bulletColRect, enemyColRect))
                    {
                        col = true;

                        enemy->dead = true;
                        enemy->vx = bullet->vx / 2; // sends the dead enemy in the same direction as the incoming bullet
                        enemy->vy = -3;             // sends the dead enemy upwards in the air, because of impact

                        Global::score += 5 * enemy->difficulty; // increase the score

                        // sound->playNote(SFX::ENEMY_DEAD);
                    }
                }
            }
            break;
        case 2:
            if (newX < 0 || newX > WIDTH || bulletHitMap(bulletColRect, map2)) // if outside screen
                col = true;
            else
            {
                // loop through all enemies
                for (std::vector<Enemy*>::iterator ite = enemies.begin(); ite != enemies.end(); ++ite)
                {
                    Enemy* enemy = *ite;

                    // If bullet hits enemy
                    //if (!enemy->dead && bullet->x >= enemy->x && bullet->x <= enemy->getRight() && bullet->y >= enemy->y && bullet->y <= enemy->getBottom()) 

                    Rectangle enemyColRect(enemy->x, enemy->y, enemy->width, enemy->height); // collision rectangle for enemy

                    if (!enemy->dead && hitTestRect(bulletColRect, enemyColRect))
                    {
                        col = true;

                        enemy->dead = true;
                        enemy->vx = bullet->vx / 2; // sends the dead enemy in the same direction as the incoming bullet
                        enemy->vy = -3;             // sends the dead enemy upwards in the air, because of impact

                        Global::score += 5 * enemy->difficulty; // increase the score

                        // sound->playNote(SFX::ENEMY_DEAD);
                    }
                }
            }
            break;
        }


        if (!col)
        {
            ++it;   // go to next element
            bullet->x += bullet->vx; // update position
        }
        else
        {
            delete bullet;
            it = bullets.erase(it); // go to next element
        }
    }

    // Check if player hits enemy
    Rectangle playerRect(player.x, player.y, player.width, player.height);
    for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
    {
        Enemy* enemy = *it;

        if (enemy->dead) continue; // only test against living enemies

        Rectangle enemyRect(enemy->x, enemy->y, enemy->width, enemy->height);

        if (hitTestRect(playerRect, enemyRect))
        {
            player.dead = true;
            player.vx = 0;
            player.vy = 4;
            --livesLeft;
            // Show the livesleft
            int seven_seg_array[] = {
                0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
            };
            shift.write(seven_seg_array[livesLeft]);

            // sound->playNote(SFX::PLAYER_DEAD);
            break;
        }
    }

    if (player.dead)
    {
        // remove all enemies (let them fall off)
        for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
        {
            Enemy* enemy = *it;
            enemy->dead = true;
            enemy->vy = 4;
        }

        if (player.y >= HEIGHT && !enemies.size()) // when all enemies are removed
        {
            if (livesLeft)
            {
                respawnPlayer(); // Respawn player if it still have lives left
                spawnEnemy();   // Spawn an enemy right away
            }
            else
            {
                if (Global::score > Global::highscores[2].score) // If new high score
                    requestStateChange(SUBMIT_HIGHSCORE);
                else
                    requestStateChange(GAME_OVER);
            }
        }
    }
}

void Game::render()
{

    if (!player.dead)
    {
        // Draw map
        switch (currentLayer)
        {
        case 1:
            drawImage(map1);
            break;
        case 2:
            drawImage(map2);
            break;
        }
        // Draw ladder
        drawImage(Image::Ladder, mapProperty.succeedPosition[0], mapProperty.succeedPosition[1], false, false, false);

        // Draw player
        drawImage(Image::Player, player.x, player.y, false, !player.facingLeft, player.dead);

        // Draw enemies

        for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
        {
            Enemy* enemy = *it;

            switch (enemy->type)
            {
            case Enemy::SIMPLE:
                drawImage(Image::EnemySimple, enemy->x, enemy->y, false, !enemy->facingLeft, enemy->dead);
                break;

            case Enemy::JUMPER:
                drawImage(Image::EnemyJumper, enemy->x, enemy->y, false, !enemy->facingLeft, enemy->dead);
                break;

            case Enemy::RUNNER:
                drawImage(Image::EnemyRunner, enemy->x, enemy->y, false, !enemy->facingLeft, enemy->dead);
                break;

            default:
                ; // should not happen, don't render
            }
        }


        // Render bullets
        for (std::vector<Point*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
        {
            int x, y;
            x = (*it)->x;
            y = (*it)->y;

            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) // Boundary check
                lcd->setPixel(x, y);
        }
    }
    else
    {
        // Print lives left
        std::stringstream ss;
        ss << "Lives left: " << livesLeft;
        lcd->printString(ss.str().c_str(), 4, 2);
    }


    // Draw pause
    if (paused)
    {
        lcd->drawRect(25, 14, 35, 13, 1); // create shadow at the bottom layer
        lcd->drawRect(24, 13, 35, 13, 0); // outline
        lcd->drawRect(25, 14, 33, 11, 2); // white fill
        lcd->printString("Pause", 27, 2); // text
    }

    // GUI
    // renderScore();
}

// Collision test between entites and map
void Game::moveWithCollisionTest(Entity* entity, const bool map[HEIGHT][WIDTH])
{
    int x = entity->x;
    int y = entity->y;
    int steps = abs(entity->vx); // how many units (pixels) the entity should move in said direction
    bool collision; // true if colliding

    // Check x-axis
    if (entity->vx > 0) // moving right
    {
        int entityRight = x + entity->width - 1; // Need to check right border of entity, since it is moving right

        while (steps--) // While it still have more movement left
        {
            collision = false;

            // Wrapping
            if (entityRight + 1 >= WIDTH)
                entityRight = -1;// wants entityRight = -1, so next check is entityRight 0*/

            for (int i = 0; i < entity->height; ++i) // Loop through all vertical points on the right hand side of the entity (y+i)
            {
                if (map[y + i][entityRight + 1]) // If moving to the right leads to collision for given y+i
                {
                    // Slope + allows player to climb to top of platform by going right if it hits close to top of wall.
                    if (!map[y + i - 1][entityRight + 1])
                    {
                        entity->vy = -1;
                    }
                    else
                    {
                        collision = true; // Then collision is true
                        break;            // Skip the for loop, no need for further testing
                    }

                }
            }

            if (collision) // If collision
                break;     // skip the while loop, entity can not move further, even though its velocity is higher
            else
                ++entityRight;  // Move entity one px to the right
        }

        // If wrap didn't work, make sure entity is on the correct side of the map
        if (entityRight < 0)
            entityRight = WIDTH - 1;

        entity->x = entityRight - (entity->width - 1); // Update entity's position. Need to set upper-left pixel.
    }
    else // moving left
    {
        while (steps--) // While still movement left
        {
            collision = false;

            // Wrap around map
            if (x - 1 < 0)
                x = WIDTH; // causes x-1 in the next check to be WIDTH - 1

            // Check for all y-positions
            for (int i = 0; i < entity->height; ++i)
            {

                if (map[y + i][x - 1])                  // If solid block
                {
                    if (!map[y + i - 1][x - 1]) // If slope or close to top of wall (=> can climb by going left).
                    {
                        entity->vy = -1;
                    }
                    else
                    {
                        collision = true;
                        break;                          // Collision detected, no further testing required
                    }
                }
            }

            if (collision)
                break;
            else
                --x;    // Move to the left if no collision is detected
        }

        x %= WIDTH; // In case wrapping caused entity to crash with wall on other side, x should be 0 instead of WIDTH (invalid).

        entity->x = x; // update position
    }

    // Check collision with map in y-direction - works the same way as the x-axis, except for other axis
    x = entity->x;
    y = entity->y;
    steps = abs(entity->vy);

    if (entity->vy > 0) // downwards
    {
        int entityBottom = y + entity->height - 1; // Need to check if bottom part collides
        while (steps--)  // Still movement left
        {
            collision = false;

            for (int i = 0; i < entity->width; ++i)  // Loop through all x-position on lower part of entity
            {
                if (map[(entityBottom + 1) % HEIGHT][x + i])       // If moving the entity one step down for a given (x+i)-position gives a collision
                {
                    collision = true;
                    break;                          // No further testing required
                }
            }

            if (collision)                          // If collision
            {
                entity->vy = 0;                      // Set vertical velocity to 0 (playe
                break;                               // Skip the while loop as the entity can not move further downwards
            }
            else                // Can safely move entity without collision
            {
                ++entityBottom; // Move entity one step down
            }
        }

        // Wrapping
        y = (entityBottom - (entity->height - 1));
        if (y >= HEIGHT)            // if completely outside map
            y = -entity->height;    // wrap to top of map

        entity->y = y; // (entityBottom - (entity->height - 1));      // Update position when done moving, remember that entity.y refers to upper part of the entity
    }
    else // moving up, check collision from top
    {
        while (steps--)  // Still movement left
        {
            collision = false;

            for (int i = 0; i < entity->width; ++i) // Check for all x-positions
            {
                int y1 = ((y - 1) + HEIGHT) % HEIGHT; // In case negative, because of wrapping

                if (map[y1][x + i])                  // If moving upwards gives collision for a given x+i
                {
                    collision = true;               // Then we have a collision
                    break;                          // No further testing needed, skip for loop
                }
            }

            if (collision)  // If collision was detected
            {
                entity->vy = 0;  // Set vertical velocity to zero
                break;          // Skip while loop as entity can not move further up
            }
            else            // If safe to move for all x-values
                --y;        // Move entity one step up
        }

        // Wrapping
        if (y + (entity->height - 1) < 0)          // completely outside map (bottom of entity over top of map)
            y = HEIGHT - 1 - entity->height - 1;     // Sets the altitude.

        entity->y = y;       // Update vertical position of entity
    }
}

bool Game::hitTestRect(Rectangle r1, Rectangle r2)
{
    return ((r1.x + r1.width > r2.x)       // r1's right edge to the right of r2's left edge
        && (r1.x < r2.x + r2.width)       // r1's left edge to the left of r2's right edge
        && (r1.y + r2.height > r2.y)      // r1's bottom lower than r2's top
        && (r1.y < r2.y + r2.height));    // r1's top higher than r2's bottom

}

bool Game::bulletHitMap(Rectangle& bulletColRect, const bool map[HEIGHT][WIDTH])
{
    for (int j = 0; j < bulletColRect.width; ++j)
    {
        if (map[bulletColRect.y][bulletColRect.x + j])
            return true;
    }

    return false;
}

void Game::moveEnemies()
{
    for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); )
    {
        Enemy* enemy = *it;

        if (!enemy->dead)
        {
            // Random movement for enemies
            if ((rand() % 100) < 20) // 20% chance
            {
                enemy->vy = -1;
            }
            else if ((rand() % 100) > 90) // 10% chance
            {
                // switch direction
                enemy->vx *= -1;
                enemy->facingLeft = (enemy->vx < 0);
            }

            switch (currentLayer)
            {
            case 1:
                moveWithCollisionTest(enemy, map1);
                break;
            case 2:
                moveWithCollisionTest(enemy, map2);
                break;
            }


            // Enemy AI
            if (enemy->y >= 0)
            {
                int nextRight = enemy->getRight() + 1;   // Next position of right edge if enemy moves to the right
                nextRight %= WIDTH; // wrapping
                bool flag = true;
                for (int i = 0; i < enemy->height && flag; ++i)  // Check for all heighs
                {
                    // Check if crashing if moving right or left. Bounds should already be limited by moveWithCollisionTest!
                    switch (currentLayer)
                    {
                    case 1:
                        if (map1[enemy->y + i][nextRight] || map1[enemy->y + i][enemy->x - 1])
                        {
                            enemy->vx *= -1; // move in opposite direction
                            enemy->facingLeft = !enemy->facingLeft; // toggle direction
                            flag = false;          // no further testing required
                        }
                        break;
                    case 2:
                        if (map2[enemy->y + i][nextRight] || map2[enemy->y + i][enemy->x - 1])
                        {
                            enemy->vx *= -1; // move in opposite direction
                            enemy->facingLeft = !enemy->facingLeft; // toggle direction
                            flag = false;         // no further testing required
                        }
                        break;
                    }
                }
            }

            ++it;   // go to next enemy
        }
        else    // if enemy is dead
        {
            enemy->y += enemy->vy;
            enemy->x += enemy->vx;

            if (enemy->y >= HEIGHT) // if outside map (and dead)
            {
                delete enemy;
                it = enemies.erase(it); // remove and go to next enemy
            }
            else
                ++it;   // go to next enemy
        }
    }
}

void Game::renderScore()
{
    int s = (Global::score < 100000) ? Global::score : 99999; // Max possible score is 99999.

    // Read digits
    int digits[5]; // max five
    // Count the number of digits in the score
    int numDigits = 0;
    do
    {
        digits[numDigits] = s % 10;
        s /= 10;
        ++numDigits;
    } while (s != 0 && numDigits < 5);


    // Draw score
    int xStart = 79;
    int xStep = 4; // width + 1
    int y = 2;
    int x;

    for (int i = 0; i < numDigits; ++i)
    {
        x = xStart - i * xStep;

        switch (digits[i])
        {
        case 1:
            drawImage(Number::One, x, y);
            break;

        case 2:
            drawImage(Number::Two, x, y);
            break;

        case 3:
            drawImage(Number::Three, x, y);
            break;

        case 4:
            drawImage(Number::Four, x, y);
            break;

        case 5:
            drawImage(Number::Five, x, y);
            break;

        case 6:
            drawImage(Number::Six, x, y);
            break;

        case 7:
            drawImage(Number::Seven, x, y);
            break;

        case 8:
            drawImage(Number::Eight, x, y);
            break;

        case 9:
            drawImage(Number::Nine, x, y);
            break;

        case 0:
        default:
            drawImage(Number::Zero, x, y);
            break;
        }

    }
}

void Game::respawnPlayer()
{
    player.x = mapProperty.respawnPosition[0];
    player.y = mapProperty.respawnPosition[1];

    player.vx = player.vy = 0;
    player.facingLeft = true;
    player.dead = false;
}