/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: prodev-ahz
 *
 * Created on August 30, 2018, 9:54 AM
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "AinoPaymentModule.h"
/*
 * Simple C Test Suite
 */

int main(int argc, char** argv) {
    int rv = 0;
    char buffer[255]= {0};
    char trx_data[2049]= {0};
    
    FILE *hFile = fopen("config.json","r");
    if (hFile == NULL) { perror("config.json"); exit(EXIT_FAILURE); };
    fseek(hFile, 0, SEEK_END);
    long fsize = ftell(hFile);
    rewind(hFile);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, hFile);
    fclose(hFile);
    
    rv = InitializeModule(string);    
    if (rv != 0) return (EXIT_FAILURE); 
    
    while(1){
        while (DetectCard(buffer)) {
            printf("No Card Detected!\n");
            sleep(1);
        }
    
        printf("Card Info : %s\n",buffer);

        double balanceBefore;
        char pan[17] = {"\0"};
        if (CheckBalance(&balanceBefore, pan) == 0) {
            printf("PAN     : %s \n", (pan));
            printf("BALANCE : %02f \n", (balanceBefore));
        }

        time_t now = time(0);
        struct tm tstruct = *localtime(&now);
        char time_str[18];
        sprintf(time_str, "%04d-%02d-%02dT%02d:%02d:%02d", tstruct.tm_year + 1900, tstruct.tm_mon + 1, tstruct.tm_mday, tstruct.tm_hour, tstruct.tm_min, tstruct.tm_sec);
        struct timespec tstart = {0, 0}, tend = {0, 0};
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        double balanceAfter;

        int ret_v = Debit(1.0, time_str, pan, &balanceBefore, &balanceAfter,trx_data);
        if (ret_v == 0) {
            clock_gettime(CLOCK_MONOTONIC, &tend);
            printf("PAN        : %s\n", pan);
            printf("BAL BEFORE : %02f\n", balanceBefore);
            printf("BAL AFTER  : %02f\n", balanceAfter);
            printf("TRANSACTION DATA : %s\n", trx_data);
            double speed = (((double) tend.tv_sec + 1.0e-9 * tend.tv_nsec) - ((double) tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));
            printf("transaction speed     : %f\n\n", speed);
        } else {
            printf("DEBIT FAILED %i \n", ret_v);
        }
    }

    
    return (EXIT_SUCCESS);
}
