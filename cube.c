#include <stdio.h>
#include <math.h>
#include <string.h>

float cubeWidth = 20;
int width = 160, height = 44;
char buffer[160 * 44];
float zBuffer[160 * 44];
int backgroundASCII = ' ';

float incrementSpeed = 0.6;
float camDistance = 100;

float A, B, C;
float x, y, z;

float calculateX(float i, float j, float k)
{
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(float i, float j, float k)
{
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);
}

float calculateZ(float i, float j, float k)
{
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

float ooz;
int xp, yp;
float K1 = 40;
int idx;

void calculateSurface(float cubeX, float cubeY, float cubeZ, int ch)
{
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + camDistance;

    ooz = 1 / z;
    xp = (int)(width / 2 + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;

    if (idx >= 0 && idx < width * height)
    {
        if (ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main()
{
    printf("\x1b[2J");
    while (1)
    {
        memset(buffer, backgroundASCII, width * height);
        memset(zBuffer, 0, width * height * 4);
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
        {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
            {
                calculateSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateSurface(cubeWidth, cubeY, cubeX, '$');
                calculateSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < width * height; k++)
        {
            putchar(k % width ? buffer[k] : 10);
        }
        A += 0.005;
        B += 0.005;
        C += 0.001;
    }
    return 0;
}