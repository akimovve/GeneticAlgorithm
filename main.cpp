#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <iomanip>



using namespace std;

const unsigned int INDIVIDS = 100;
void generatePop(vector<float> &pop);
float func(float x, float y);
void fitness(vector<float> &popX, vector<float> &popY, vector<float> &fitnessRes, vector<float> &nextPopulationX, vector<float> &nextPopulationY, vector<float> &resultVector);
float amountOfFitness(vector<float> &fit);
void sortVec(vector<float> &popX, vector<float> &popY, vector<float> &fitness);
void nextPop(vector<float> &fitnessSum, vector<float> &popX, vector<float> &popY, vector<float> &nextPopulationX, vector<float> &nextPopulationY, float total);
int selection(vector<float> &fitnessSum, float total);
void crossover(vector<float> &nextPopX, vector<float> &nextPopY, vector<float> &popX, vector<float> &popY, vector<int> &parent_1, vector<int> &parent_2);
void mutation(vector<float> &nextPopX, vector<float> &nextPopY);
int endOfAlgorithm (vector<float> &fitnessRes);
void changeHelper(vector<float> &x, int i, int step, int fl);
void clearVector(vector<float> &x);
void output(vector<float> &pop);
void output(vector<int> &pop);
void nextLine();




int main() {
    srand((unsigned)time(nullptr));
    vector<float>populationX;
    vector<float>populationY;
    vector<float>fitnessResult;
    vector<float>nextPopulationX;
    vector<float>nextPopulationY;
    vector<float>resultVector = {0}; //Вектор для записи ответа. (X;Y)=Z.

    generatePop(populationX);
    generatePop(populationY);
    cout << "X \t\t";
    output(populationX);
    cout << "Y \t\t";
    output(populationY);
    int rep = 0; //Считает количество итераций.

     do {
        fitness(populationX, populationY, fitnessResult, nextPopulationX, nextPopulationY, resultVector);
        for (int i = 0; i < INDIVIDS; i++) { //Присваивание нынешнему поколению новое. Эффект взрросления детей.
            populationX[i] = nextPopulationX[i];
            populationY[i] = nextPopulationY[i];
        }
        clearVector(nextPopulationX);
        clearVector(nextPopulationY);
        clearVector(fitnessResult);
        rep++;

        nextLine(); //Вывод для наглядности завершения цикла.
     } while (endOfAlgorithm(fitnessResult) != 1);

    cout << "Without round (X) = " << resultVector[0] << endl;
    cout << "Without round (Y) = " << resultVector[1] << endl;
    cout << "Without round (Z) = " << resultVector[2] << endl << endl << endl << "________________________________" << endl;

    cout << "Number of iterations:\t" << rep << endl << endl << "ANSWER:\t";
    cout << fixed << setprecision(1) << "(" << round(resultVector[0]) << ";" << resultVector[1] << ") = " << floor(resultVector[2]) << endl << endl << endl;

    return 0;
}



void generatePop(vector<float> &pop) {
    for (int i = 0; i < INDIVIDS; i++) {
        pop.push_back((float) ((float) rand() / (RAND_MAX + 1.0) > 0.5 ? rand() : -1 * rand()) / RAND_MAX + rand() % 5);
    }
}

float func(float x, float y) {
    return (float)(pow(x, 3) + 8 * pow(y, 3) - 6 * x * y + 5);

}

void fitness(vector<float> &popX, vector<float> &popY, vector<float> &fitnessRes, vector<float> &nextPopulationX, vector<float> &nextPopulationY, vector<float> &resultVector) { //Считает приспособленность каждой хромосомы, а также их сумму.

    for (int i = 0; i < INDIVIDS; i++) {
        fitnessRes.push_back(func(popX[i], popY[i]));
    }

    cout << "Z\t\t";
    output(fitnessRes);
    cout << endl;

    sortVec(popX, popY, fitnessRes);
    cout << "X sort\t";
    output(popX);
    cout << "Y sort\t";
    output(popY);
    cout << "Z sort\t";
    output(fitnessRes);
    cout << endl;

    float fit = 0;
    vector<float>fitnessSum; //Прибавление к каждому элементу вектора Z сумму предыдущих для выбора наилучшей особи для селекции.
    clearVector(fitnessSum);

    for (unsigned i = 0; i < INDIVIDS; i++) {
        fit += fitnessRes[i];
        fitnessSum.push_back(fit);
    }

    cout << "fitnessSum\t";
    output(fitnessSum);
    cout << endl << ".................................." << endl;
    cout << "X fin\t";
    output(popX);
    cout << endl;
    cout << "Y fin\t";
    output(popY);
    cout << ".................................." << endl;

    clearVector(resultVector);
    resultVector[0] = popX[0];
    resultVector[1] = popY[0];
    resultVector[2] = fitnessRes[0];




    nextPop(fitnessSum, popX, popY, nextPopulationX, nextPopulationY, amountOfFitness(fitnessRes));
    cout << endl << ".................................." << endl;
    cout << "Z fin\t";
    output(fitnessRes);
    cout << ".................................." << endl;

    clearVector(fitnessSum);
}

float amountOfFitness(vector<float> &fit) { //Вычисление общей суммы приспособленностей функции.
    float res = 0;
    for (int i = 0; i < INDIVIDS; i++) {
        res += fit[i];
    }
    return res;
}

void sortVec(vector<float> &popX, vector<float> &popY, vector<float> &fitness) { //Сортирует вектор по возрастанию.
    for (int step = (INDIVIDS / 2); step > 0; step /= 2) { //Цикл для шага.
        for (int i = 0; i < (INDIVIDS - step); i++) { //Цикл для перебора элементов массива.
            if (fitness[i] > fitness[i + step]) {

                changeHelper(popX, i, step, 1);
                changeHelper(popY, i, step, 1);
                changeHelper(fitness, i, step, 1);

                for (int j = i; j >= step; j--) { //Цикл для переставленного элемента массива с целью дальнейшего его сдвига влево.
                    if (fitness[j] < fitness[j - step]) {

                        changeHelper(popX, i, step, 0);
                        changeHelper(popY, i, step, 0);
                        changeHelper(fitness, i, step, 0);

                    }
                }
            }
        }
    }
}

void nextPop(vector<float> &fitnessSum, vector<float> &popX, vector<float> &popY, vector<float> &nextPopulationX, vector<float> &nextPopulationY, float total) {


    vector<int> parent_1;
    vector<int> parent_2;
    for (unsigned i = 0; i < INDIVIDS; i++) {
        parent_1.push_back(selection(fitnessSum, total));
        parent_2.push_back(selection(fitnessSum, total));
        if (parent_1[i] == parent_2[i]) {
            if ((rand()/(RAND_MAX + 1.0) < 0.5)) {
                if ((rand()/(RAND_MAX + 1.0) < 0.5)) {
                    parent_1[i] = selection(fitnessSum, total);
                } else {
                    parent_2[i] = selection(fitnessSum, total);
                }
            }
        }
    }
    cout << endl << endl << "1st parents:\t\t";
    output(parent_1);
    cout << "2nd parents:\t\t";
    output(parent_2);
    cout << endl;
    crossover(nextPopulationX, nextPopulationY, popX, popY, parent_1, parent_2);


    cout << endl << "next population X\t\t";
    output(nextPopulationX);
    cout << "next population Y\t\t";
    output(nextPopulationY);

    for (unsigned i = 0; i < INDIVIDS; i++) {
        popX[i] = nextPopulationX[i];
        popY[i] = nextPopulationY[i];
    }
    parent_1.clear();
    parent_2.clear();
    clearVector(nextPopulationX);
    clearVector(nextPopulationY);
}

int selection(vector<float> &fitnessSum, float total) { //Методом рулетки.
    float randomFitnessToChooseParent = (float)(rand())/RAND_MAX * (total/3);
    int firstFitnessIndex = 0;
    int lastFitnessIndex = INDIVIDS - 1;
    int middle = (int)((firstFitnessIndex + lastFitnessIndex)/2);
    int parentIndex = INT_MIN;

    do {
        (randomFitnessToChooseParent > fitnessSum[middle])? firstFitnessIndex = middle:lastFitnessIndex = middle;
        middle = (int)((firstFitnessIndex + lastFitnessIndex)/2);
        if (abs(firstFitnessIndex - lastFitnessIndex) == 1) {
            parentIndex = lastFitnessIndex;
        }
    } while ((parentIndex < 0) && (firstFitnessIndex <= lastFitnessIndex));
    return parentIndex;
}

void crossover(vector<float> &nextPopX, vector<float> &nextPopY, vector<float> &popX, vector<float> &popY, vector<int> &parent_1, vector<int> &parent_2) {

    for (unsigned i = 0; i < INDIVIDS; i++) {
        if ((rand()/(RAND_MAX + 1.0)) > 0.6) {
            nextPopX.push_back(popX[parent_1[i]]);
            nextPopY.push_back(popY[parent_2[i]]);
        } else {
            if ((rand()/(RAND_MAX + 1.0)) < 0.6) {
                nextPopX.push_back(popX[parent_1[i]]);
                nextPopY.push_back(popY[parent_1[i]]);
            } else {
                nextPopX.push_back(popX[parent_2[i]]);
                nextPopY.push_back(popY[parent_2[i]]);
            }
        }
    }
    mutation(nextPopX, nextPopY);
}

void mutation(vector<float> &nextPopX, vector<float> &nextPopY) {
    for (unsigned i = 0; i < INDIVIDS; i++) {
        if ((rand()/(RAND_MAX + 1.0)) < 0.6) {
            if ((rand()/(RAND_MAX + 1.0)) < 0.5) {
                nextPopX[i] += 0.1;
                nextPopY[i] += 0.1;
            } else {
                nextPopX[i] -= 0.7;
                nextPopY[i] -= 0.7;
            }
        }
    }
}

int endOfAlgorithm (vector<float> &fitnessRes) {
    float res = 0;
    for (unsigned i = 0; i < floor(INDIVIDS/2); i++) {
        res += fitnessRes[i];
    }
    for (unsigned i = 0; i < floor(INDIVIDS/2); i++) {
        res -= fitnessRes[0];
    }
    return (res == 0) || ((res > 0) && (res < 1.0))? 1:0;
}

void changeHelper(vector<float> &x, int i, int step, int fl) { //Используется для обмена местами элементов вектора.
    float helper;
    switch (fl) {
        case 1:
            helper = x[i];
            x[i] = x[i + step];
            x[i + step] = helper;
            break;
        case 0:
            helper = x[i];
            x[i] = x[i - step];
            x[i - step] = helper;
            break;

        default:
            break;
    }
}

void clearVector(vector<float> &x) {
    x.clear();
}

void output(vector<float> &pop) {
    for (float i : pop) {
        cout << fixed << i << "\t\t";
    }
    cout << endl;
}

void output(vector<int> &pop) {
    for (int i : pop) {
        cout << fixed << i << "\t\t";
    }
    cout << endl;
}

void nextLine() {
    cout << endl << endl << "next next next next next next next next next next next next next next next next next next next next next next next next next next next next next next next next next" << endl << endl << endl;
}

