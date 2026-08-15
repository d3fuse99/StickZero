#include "game.h"

struct Point {
    int x;
    int y;
};

void startSnakeGame() {
    int snakeX[50];
    int snakeY[50];
    int length = 4;
    int dirX = 1;
    int dirY = 0;

    for (int i = 0; i < length; i++) {
        snakeX[i] = 10 - i;
        snakeY[i] = 5;
    }

    Point food = {15, 7};
    int score = 0;
    bool gameOver = false;

    M5.Lcd.fillScreen(BLACK);

    while (!gameOver) {
        M5.update();

        if (M5.BtnB.wasPressed()) {
            int temp = dirX;
            dirX = -dirY;
            dirY = temp;
        }

        if (M5.BtnA.wasPressed()) {
            int temp = dirX;
            dirX = dirY;
            dirY = -temp;
        }

        int newX = snakeX[0] + dirX;
        int newY = snakeY[0] + dirY;

        if (newX < 0 || newX >= 24 || newY < 0 || newY >= 13) {
            gameOver = true;
            break;
        }

        for (int i = 0; i < length; i++) {
            if (snakeX[i] == newX && snakeY[i] == newY) {
                gameOver = true;
                break;
            }
        }

        if (newX == food.x && newY == food.y) {
            score += 10;
            if (length < 48) length++;
            food.x = random(1, 23);
            food.y = random(1, 12);
        } else {
            M5.Lcd.fillRect(snakeX[length - 1] * 10, snakeY[length - 1] * 10, 9, 9, BLACK);
        }

        for (int i = length - 1; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
        snakeX[0] = newX;
        snakeY[0] = newY;

        M5.Lcd.fillRect(food.x * 10, food.y * 10, 9, 9, RED);
        M5.Lcd.fillRect(snakeX[0] * 10, snakeY[0] * 10, 9, 9, GREEN);

        M5.Lcd.fillRect(0, 0, 240, 10, NAVY);
        M5.Lcd.setTextColor(WHITE, NAVY);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(5, 1);
        M5.Lcd.printf("SNAKE GAME | Score: %d", score);

        delay(120);
    }

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(50, 45);
    M5.Lcd.println("GAME OVER!");
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(50, 75);
    M5.Lcd.printf("Final Score: %d", score);
    delay(2000);
}