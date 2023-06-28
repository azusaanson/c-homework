#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dailyTemperature {
    int year;
    int month;
    int day;
    double maxTemperature;
    double minTemperature;
} DAILYTEMPERATURE;

typedef struct summerdayCount {
    int normal;
    int hot;
    int extreme;
} SUMMERDAYCOUNT;

void readCsvAndCount(char inFileName, DAILYTEMPERATURE *dt, SUMMERDAYCOUNT *sc);
void initCount(SUMMERDAYCOUNT *sc);
void classifySummerday(DAILYTEMPERATURE *dt, SUMMERDAYCOUNT *sc);
void writeCount(char outFileName, SUMMERDAYCOUNT sc[]);

int main(void) {
    char inFileNames[] = {"1922.csv", "2022.csv"};
    char outFileNames[] = {"out1922.csv", "out2022.csv"};

    DAILYTEMPERATURE dailyTemp;
    SUMMERDAYCOUNT summerdayCnt[12];

    for (int i = 0; i < 2; i++) {
        readCsvAndCount(inFileNames[i], &dailyTemp, summerdayCnt);
        writeCount(outFileNames[i], summerdayCnt);
    }

    return 0;
}

void readCsvAndCount(char inFileName, DAILYTEMPERATURE *dt, SUMMERDAYCOUNT *sc) {
    FILE *fp;
    fp = fopen(inFileName, "r");
    if (fp == NULL) {
        printf(inFileName + "のオープンに失敗しました\n");
        exit(1);
    }
    printf(inFileName + "からデータ読み込み開始\n");

    initCount(sc);
    while (fscanf(fp, "%d,%d,%lf,%lf\n",
        dt->year, dt->month, dt->day, dt->maxTemperature, dt->minTemperature) != EOF){
        classifySummerday(dt, sc);
    }

    fclose(fp);
    printf(inFileName + "ファイル処理完了\n");
    return;
}

void initCount(SUMMERDAYCOUNT *sc) {
    for (int i = 0; i < 12; i++) {
        sc[i].normal = 0;
        sc[i].hot = 0;
        sc[i].extreme = 0;
    }
    return;
}

void classifySummerday(DAILYTEMPERATURE *dt, SUMMERDAYCOUNT *sc) {
    int monthIndex = dt->month - 1;
    double maxTemp = dt->maxTemperature;

    if (maxTemp >= 35) {
        sc[monthIndex].extreme++;
        return;
    }
    if (maxTemp >= 30) {
        sc[monthIndex].hot++;
        return;
    }
    if (maxTemp >= 25) {
        sc[monthIndex].normal++;
        return;
    }
    return;
}

void writeCount(char outFileName, SUMMERDAYCOUNT sc[]) {
    FILE *fp;
    fp = fopen(outFileName, "w");
    if (fp == NULL) {
        printf(outFileName + "のオープンに失敗しました\n");
        exit(1);
    }
    printf(outFileName + "ファイル書き出し開始\n");

    for (int i = 0; i < 12; i++) {
        fprintf(fp, "%d,%d,%d,%d\n",
            (i+1) + "月", sc[i].normal, sc[i].hot, sc[i].extreme);
    }

    fclose(fp);
    printf(outFileName + "ファイルの書き込みが完了しました\n");
    return;
}
