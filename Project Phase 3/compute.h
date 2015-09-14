/*
 * compute.h
 *
 *  Created on: Mar 3, 2015
 *      Author: bwatt89
 *      Edited by: Andrew Gates and Brandon Watt
 */

#ifndef COMPUTE_H_
#define COMPUTE_H_

int compute_main_menu();
int compute_menu(void);
int print_compute_menu(void);

void getData(double *, FileHeaderPtr, char *, int *);
void edit_file(double *, FileHeaderPtr, char *);
int compute_update(void);

int compute_mean(void);

int compute_median(void);
void median_sort(double*, long);
int compare (const void *, const void *);

int compute_variance(void);

int compute_std_dev(void);

#endif /* COMPUTE_H_ */
