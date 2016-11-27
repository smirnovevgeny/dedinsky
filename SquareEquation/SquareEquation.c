//
// Created by evgeny on 25.09.16.
//
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
//limits.h


double EPS = 1e-6;

enum TYPES {
NO_ROOTS     = -1,
ALL_ROOTS    = -2,
NULL_POINTER = -3
};


static inline bool isZero(double x) {
    return fabs(x) < EPS;
}


static inline double checkZero(double x) {
    return (isZero(x))? 0 : x;
}


static inline int solveLinear(double b, double c, double *x) {
    if (isZero(b)) {
        if (!isZero(c)) {
            return NO_ROOTS;
        } else {
            return ALL_ROOTS;
        }
    } else {
        if (x != NULL) {
            *x = checkZero(-c / b);
            return 1;
        } else {
            return NULL_POINTER;
        }
    }
}


int solveSquare(double a, double b, double c, double *x1, double *x2) {
    double D = 0;
    if (!isZero(a)) {
        D = pow(b, 2) - 4 * a * c;
        if (D >= 0) {
            if (x1 != NULL) {
                *x1 = checkZero((-b - sqrt(D)) / (2 * a));
            } else {
                return NULL_POINTER;
            }
            if (!isZero(D)) {
                if (x2 != NULL) {
                    *x2 = checkZero((-b + sqrt(D)) / (2 * a));
                    return 2;
                } else {
                    return NULL_POINTER;
                }
            }
            return 1;
        } else {
            return NO_ROOTS;
        }
    } else {
        return solveLinear(b, c, x1);
    }
}


int main() {
    printf("Введите коэффициенты a, b, c квадратного уравнения a * x^2 + b * x + c = 0.\n");
    double a = 0, b = 0, c = 0, x1, x2;
    char err;
    int roots_n;
    int arg_n = scanf("%lg %lg %lg%c", &a, &b, &c, &err);
    if (arg_n == 4 && err != '\n' || arg_n < 3)  {
        printf("Введённые значения не явлются действительными числами.\n");
        return 1;
    }
    roots_n = solveSquare(a, b, c, &x1, &x2);
    switch (roots_n) {
        case 2:
            printf("x1 = %lg\n", x1);
            printf("x2 = %lg", x2);
            break;
        case 1:
            printf("x = %lg", x1);
            break;
        case -1:
            printf("Уравнение не имеет решения.");
            break;
        case -2:
            printf("Уравнению удовлетворяет любое число.");
            break;
        case -3:
            return 2;
        default:
            break;
    }
    return 0;
}