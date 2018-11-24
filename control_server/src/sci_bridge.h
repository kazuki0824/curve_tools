/*
 * sci_bridge.h
 *
 *  Created on: 2018/11/07
 *      Author: kazuki
 */

#ifndef SCI_BRIDGE_H_
#define SCI_BRIDGE_H_

int init_sci(char* scriptPath);
int deinit_sci();
double * read_matrix(const char variableToBeRetrieved[], int * rowCount, int * colCount);

int process_ref(double velo, double accel, double duty, double Tf);
int process_C1(double Kf[3]);

int Init_MotorModel();

#endif /* SCI_BRIDGE_H_ */
