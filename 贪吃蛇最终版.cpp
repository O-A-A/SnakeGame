#include <ege.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <conio.h>
using namespace ege;
using namespace std;

const int WIDTH = 400 * 2;
const int HEIGHT = 400 * 2;
const int SEG_SIZE = 20;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

struct SnakeSegment {
    int x, y;
};


struct Obstacle {
    int x, y; // 障碍物的位置
    int length; // 障碍物的长度
    Direction direction; // 障碍物的方向（水平或垂直）
};


class Menu {
public:
    Menu(){};
	

    ~Menu() {
        delimage(backgroundImage);
        delimage(playButtonImage);
    }

    void run() {
        ege::initgraph(WIDTH, HEIGHT); 
		 loadImages() ;
        while (true) {
            draw();
            if (checkButtonClick()) {
                break; // 如果点击按钮，退出菜单
            }
            ege::delay_fps(60);
        }
        ege::closegraph();
    }

private:
    PIMAGE backgroundImage;
    PIMAGE playButtonImage;

    void loadImages() {
        backgroundImage = newimage();
        ege::getimage(backgroundImage, "C:/Users/34490/Desktop/snake_game/menu6.jpg");
        
        playButtonImage = newimage();
        ege::getimage(playButtonImage, "C:/Users/34490/Desktop/snake_game/bottom2.png");
    }

    void draw() {
        ege::cleardevice();
      
        ege::putimage(0, 0, backgroundImage); // 绘制背景

        // 绘制“开始游戏”按钮
        int buttonX = (WIDTH - ege::getwidth(playButtonImage)) / 2;
        int buttonY = HEIGHT / 2+100;
        ege::putimage(buttonX, buttonY, playButtonImage);
    }

    bool checkButtonClick() {
        if (ege::kbhit()) {
            ege::getch(); // 消耗键盘输入
        }

        // 检查鼠标输入
        mouse_msg msg = ege::getmouse(); // 不传递参数，直接获取鼠标信息
        if (msg.is_left()) {
            int buttonX = (WIDTH - ege::getwidth(playButtonImage)) / 2;
            int buttonY = HEIGHT / 2+100;
            int buttonWidth = ege::getwidth(playButtonImage);
            int buttonHeight = ege::getheight(playButtonImage);

            // 检查鼠标是否在按钮范围内
            if (msg.x >= buttonX && msg.x <= buttonX + buttonWidth &&
                msg.y >= buttonY && msg.y <= buttonY + buttonHeight) {
                return true; // 点击了按钮
            }
        }
        return false;
    }
};



class SnakeGame {
public:
    SnakeGame() : direction(STOP), score(0), gameOver(false),paused(false) {
        snake.push_back({ WIDTH / 2, HEIGHT / 2 });
        placeFood();
        placeUpFood();
        placeDownFood();
       placeObstacles();
       placeBomb();
        srand(static_cast<unsigned>(time(0)));
    }
	~SnakeGame() {
    // 释放蛇头和蛇身的图像资源
    if (snakeHeadImageUp) ege::delimage(snakeHeadImageUp);
    if (snakeHeadImageDown) ege::delimage(snakeHeadImageDown);
    if (snakeHeadImageRight) ege::delimage(snakeHeadImageRight);
    if (snakeHeadImageLeft) ege::delimage(snakeHeadImageLeft);
    if (snakeBodyImage) ege::delimage(snakeBodyImage);
    if (foodImage) ege::delimage(foodImage);
    if (backgroundImage) ege::delimage(backgroundImage);
    if (BackgroundImage) ege::delimage(BackgroundImage);  // 释放菜单背景图片
}


void run() {
    ege::initgraph(WIDTH, HEIGHT);
    loadImages();
	 setfont(40, 0, "微软雅黑");
	  
     while (true) {
        if (!gameOver) {
            if (!paused) {
                input();
                logic();
                draw();
                ege::delay_fps(currentSpeed);
            } else {
                drawPaused(); // 游戏暂停时绘制提示
                input(); // 在暂停状态下仍然检测输入以便取消暂停
            }
        } else {
            drawGameOverMenu(); // 游戏结束时显示菜单
            if (!playAgain) {
                break; // 退出游戏
            }
            resetGame(); // 重新开始游戏
        }
    }
    ege::closegraph();
    printf("游戏结束！你的得分: %d\n", score);
}


private:
    PIMAGE snakeHeadImageUp; 
	PIMAGE snakeHeadImageDown;
	PIMAGE snakeHeadImageRight;
	PIMAGE snakeHeadImageLeft; // Snake head image
    PIMAGE snakeBodyImage;  // Snake body image
    PIMAGE foodImage;   // Food image
	PIMAGE backgroundImage;    
	PIMAGE BackgroundImage;  // 结束菜单背景图片
    vector<SnakeSegment> snake;
    SnakeSegment food;
    Direction direction;
    int score;
    bool gameOver;
	bool playAgain;
	vector<Obstacle> obstacles; // 存储障碍物
	PIMAGE wall;
	struct Fruit {
    	int x, y;
	};

Fruit speedFruit;
Fruit slowFruit;
int normalSpeed = 10; // 默认速度
int currentSpeed = normalSpeed;
int speedUpDuration = 0; // 加速效果的持续时间
int slowDownDuration = 0; // 减速效果的持续时间
PIMAGE speedFruitImage;
PIMAGE slowFruitImage;
	 bool paused;
	 struct Bomb {
    	int x, y;
	};

	Bomb bomb; // 炸弹的位置
	PIMAGE bombImage; // 炸弹的图片
	int elapsedTime;
	 
	 
	 
	 
	 
	 
	
    void loadImages() {
        snakeHeadImageUp = newimage();
        ege::getimage(snakeHeadImageUp, "C:/Users/34490/Desktop/snake_game/snakeHeadUp.png");
         snakeHeadImageDown = newimage();
        ege::getimage(snakeHeadImageDown, "C:/Users/34490/Desktop/snake_game/snakeHeadDown.png");
         snakeHeadImageRight = newimage();
        ege::getimage(snakeHeadImageRight, "C:/Users/34490/Desktop/snake_game/snakeHeadRight.png");
         snakeHeadImageLeft = newimage();
        ege::getimage(snakeHeadImageLeft, "C:/Users/34490/Desktop/snake_game/snakeHeadLeft.png");
        snakeBodyImage = newimage();
        ege::getimage(snakeBodyImage, "C:/Users/34490/Desktop/snake_game/snakeTail.png"); // Load body image
        foodImage = newimage();
        ege::getimage(foodImage, "C:/Users/34490/Desktop/snake_game/china.png");
          backgroundImage = newimage(); // 初始化背景图像
        ege::getimage(backgroundImage, "C:/Users/34490/Desktop/snake_game/background.jpg"); // 加载背景图像
       BackgroundImage = newimage();
       ege::getimage(BackgroundImage, "C:/Users/34490/Desktop/snake_game/endbackground.jpg");
       wall= newimage();
        ege::getimage(wall, "C:/Users/34490/Desktop/snake_game/wall.png");
         speedFruitImage = newimage();
    ege::getimage(speedFruitImage, "C:/Users/34490/Desktop/snake_game/speedFruit.png");

    slowFruitImage = newimage();
    ege::getimage(slowFruitImage, "C:/Users/34490/Desktop/snake_game/downFruit.jpg");
      bombImage = newimage();
    ege::getimage(bombImage, "C:/Users/34490/Desktop/snake_game/bomb.png");
    }

    void draw() { 

        ege::cleardevice(); 
		
          ege::putimage(0, 0, backgroundImage);
       
       
        ege::putimage(bomb.x, bomb.y, bombImage);
        // Draw the head
        switch (direction) {
        case LEFT:
            ege::putimage(snake.front().x, snake.front().y, snakeHeadImageLeft);
            break;
        case RIGHT:
            ege::putimage(snake.front().x, snake.front().y, snakeHeadImageRight);
            break;
        case UP:
            ege::putimage(snake.front().x, snake.front().y, snakeHeadImageUp);
            break;
        case DOWN:
            ege::putimage(snake.front().x, snake.front().y, snakeHeadImageDown);
            break;
        default:
        	ege::putimage(snake.front().x, snake.front().y, snakeHeadImageUp);
            break;
    }
        ege::putimage(speedFruit.x, speedFruit.y, speedFruitImage);

    // 绘制减速果实
   		 ege::putimage(slowFruit.x, slowFruit.y, slowFruitImage);
        
        // Draw the body
        for (size_t i = 1; i < snake.size(); i++) {
            ege::putimage(snake[i].x, snake[i].y, snakeBodyImage);
        }

        // Draw food image
        ege::putimage(food.x, food.y, foodImage);

        char scoreText[20];
        sprintf(scoreText, "得分: %d", score);
        ege::outtextxy(10, 10, scoreText);
        
        
        
         for (const auto& obs : obstacles) {
        if (obs.direction == LEFT || obs.direction == RIGHT) {
            for (int i = 0; i < obs.length / SEG_SIZE; i++) {
                ege::putimage(obs.x + i * SEG_SIZE, obs.y, wall); // 使用蛇身图片绘制
            }
        } else {
            for (int i = 0; i < obs.length / SEG_SIZE; i++) {
                ege::putimage(obs.x, obs.y + i * SEG_SIZE, wall); // 使用蛇身图片绘制
            }
        }
    }
        
        
        
    }

    void input() {
    if (gameOver) return; // 如果游戏结束，直接返回

    if (ege::kbhit()) {
        switch (ege::getch()) {
            case '1': direction = LEFT; break;
            case '3': direction = RIGHT; break;
            case '5': direction = UP; break;
            case '2': direction = DOWN; break;
            case '7': gameOver = true; break; // 可以结束游戏
            case '9': paused = !paused; break; // 按下 'p' 键来暂停或恢复游戏
        }
    }
}


    void logic() {
        SnakeSegment newHead = snake.front();
        switch (direction) {
            case LEFT: newHead.x -= SEG_SIZE; break;
            case RIGHT: newHead.x += SEG_SIZE; break;
            case UP: newHead.y -= SEG_SIZE; break;
            case DOWN: newHead.y += SEG_SIZE; break;
        }

        // Check boundaries
        if (newHead.x >= WIDTH || newHead.x < 0 || newHead.y >= HEIGHT || newHead.y < 0) {
            gameOver = true;
        }

        // Check self-collision
        for (size_t i = 1; i < snake.size(); i++) {
            if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
                gameOver = true;
            }
        }

		snake.insert(snake.begin(), newHead);
		
		 // Update elapsed time
        elapsedTime++;

        // 更新障碍物
        if (elapsedTime >= 60) {
            obstacles.clear();  // 清空当前障碍物
            placeObstacles();   // 重新放置障碍物
            elapsedTime = 0;    // 重置计时器
        }

        if (newHead.x == food.x && newHead.y == food.y) {
        score += 10;  // 增加分数
        placeFood();  // 重新放置普通食物
        // 不删除尾部，蛇变长
    } 
    else if (newHead.x == speedFruit.x && newHead.y == speedFruit.y) {
        score += 20;           // 吃到加速果实，加20分
        currentSpeed = 15;     // 增加当前速度
        speedUpDuration = 100; // 持续 100 帧
        placeUpFood();         // 重新生成新的加速果实
        // 不删除尾部，蛇变长
    } 
    else if (newHead.x == slowFruit.x && newHead.y == slowFruit.y) {
        score += 10;           // 吃到减速果实，加10分
        currentSpeed = 5;      // 减慢当前速度
        slowDownDuration = 100; // 持续 100 帧
        placeDownFood();       // 重新生成新的减速果实
        // 不删除尾部，蛇变长
    } 
   else if (newHead.x == bomb.x && newHead.y == bomb.y) {
    // 吃到炸弹的处理逻辑
    score -= 10; // 扣10分
    int shrinkLength = 1; // 要缩短的单位长度
    if (snake.size() > shrinkLength) {
        for (int i = 0; i < shrinkLength; ++i) {
            snake.pop_back(); // 缩短蛇身
        }
    } else {
        gameOver = true; // 如果蛇长度小于等于缩短长度，游戏结束
    }
    placeBomb(); // 重新放置炸弹
	}
    else {
        // 没有吃到任何果实时，删除尾部，维持长度
        snake.pop_back();
    }
         if(score<0) {
         	score=0;
		 gameOver = true;
		 }
        
   
    
         for (const auto& obs : obstacles) {
        if (obs.direction == LEFT || obs.direction == RIGHT) {
            for (int i = 0; i < obs.length / SEG_SIZE; i++) {
                if (newHead.x == obs.x + i * SEG_SIZE && newHead.y == obs.y) {
                    gameOver = true; // 碰撞，游戏结束
                }
            }
        } else {
            for (int i = 0; i < obs.length / SEG_SIZE; i++) {
                if (newHead.x == obs.x && newHead.y == obs.y + i * SEG_SIZE) {
                    gameOver = true; // 碰撞，游戏结束
                }
            }
        }
    }
        

    // 更新加速和减速效果的持续时间
    if (speedUpDuration > 0) {
        speedUpDuration--;
        if (speedUpDuration == 0) {
            currentSpeed = normalSpeed;  // 恢复默认速度
        }
    }

    if (slowDownDuration > 0) {
        slowDownDuration--;
        if (slowDownDuration == 0) {
            currentSpeed = normalSpeed;  // 恢复默认速度
        }
    }
        
        
    }

    void placeFood() {
        food.x = (rand() % (WIDTH / SEG_SIZE)) * SEG_SIZE;
        food.y = (rand() % (HEIGHT / SEG_SIZE)) * SEG_SIZE;
 
    }
    
    
    void placeUpFood(){ 
	      // 放置加速果实
    speedFruit.x = (rand() % (WIDTH / SEG_SIZE)) * SEG_SIZE;
    speedFruit.y = (rand() % (HEIGHT / SEG_SIZE)) * SEG_SIZE;
    	
	}
    
     void placeDownFood(){ 
      // 放置减速果实
    slowFruit.x = (rand() % (WIDTH / SEG_SIZE)) * SEG_SIZE;
    slowFruit.y = (rand() % (HEIGHT / SEG_SIZE)) * SEG_SIZE;
}

    void placeBomb() {
    bomb.x = (rand() % (WIDTH / SEG_SIZE)) * SEG_SIZE;
    bomb.y = (rand() % (HEIGHT / SEG_SIZE)) * SEG_SIZE;
}
    
    
    
    void placeObstacles() {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < 5; i++) { // 生成5个障碍物
        Obstacle obs;
        obs.length = (rand() % 3 + 2) * SEG_SIZE; // 随机长度（2-4个单位）
        obs.direction = static_cast<Direction>(rand() % 2); // 随机方向（0:水平, 1:垂直）

        if (obs.direction == LEFT || obs.direction == RIGHT) {
            obs.x = (rand() % (WIDTH / SEG_SIZE)) * SEG_SIZE;
            obs.y = (rand() % (HEIGHT / SEG_SIZE)) * SEG_SIZE;
        } else {
            obs.x = (rand() % (WIDTH / SEG_SIZE)) * SEG_SIZE;
            obs.y = (rand() % (HEIGHT / SEG_SIZE)) * SEG_SIZE;
        }
        
        obstacles.push_back(obs); // 添加到障碍物列表
    }
}
    
    
    void drawPaused() {
    ege::outtextxy(WIDTH / 2 - 100, HEIGHT / 2, "游戏已暂停");
    ege::delay_fps(30); // 降低帧率以节省资源
}
    
    
    
    
    
    
   void drawGameOverMenu() {
    ege::cleardevice();
    ege::putimage(0, 0, BackgroundImage); 

   

    ege::outtextxy(WIDTH / 2 - 100, HEIGHT / 2 - 300, "游戏结束");
    ege::outtextxy(WIDTH / 2 - 100, HEIGHT / 2 - 200, "得分: ");
    
    char scoreText[20];
    sprintf(scoreText, "%d", score);
    ege::outtextxy(WIDTH / 2, HEIGHT / 2 - 200, scoreText);

    ege::outtextxy(WIDTH / 2 - 100, HEIGHT / 2 - 100, "再来一局");
    ege::outtextxy(WIDTH / 2 - 100, HEIGHT / 2, "退出游戏");

    // 等待鼠标点击
    while (true) {
        mouse_msg msg = ege::getmouse();
        if (msg.is_left()) {
            if (msg.x >= WIDTH / 2 - 100 && msg.x <= WIDTH / 2 + 100 &&
                msg.y >= HEIGHT / 2 - 100 && msg.y <= HEIGHT / 2 - 60) {
                playAgain = true;
                return; // 点击“再来一局”，返回
            }
            if (msg.x >= WIDTH / 2 - 100 && msg.x <= WIDTH / 2 + 100 &&
                msg.y >= HEIGHT / 2 && msg.y <= HEIGHT / 2 + 40) {
                playAgain = false;
                return; // 点击“退出游戏”，返回
            }
        }
    }
}








    
    void resetGame() {
    gameOver = false; // 重置游戏状态
    snake.clear(); // 清空蛇的状态
    snake.push_back({ WIDTH / 2, HEIGHT / 2 }); // 重新初始化蛇的位置
    score = 0; // 重置得分
    placeFood(); // 重新放置食物
    direction = STOP; // 重置方向
    obstacles.clear();  // 清空上局障碍物
    placeObstacles();   // 重新生成障碍物
    placeBomb();
}

    
    
    
    
    
    
    
};

int main() {
	Menu menu;
    menu.run();
    SnakeGame game;
    game.run();
    return 0;
}