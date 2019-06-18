#include <stdio.h>
#include <math.h>
#include <pthread.h>

/******************************************************************************
 * This program takes an initial estimate of m and c and finds the associated 
 * rms error. It is then as a base to generate and evaluate 8 new estimates, 
 * which are steps in different directions in m-c space. The best estimate is 
 * then used as the base for another iteration of "generate and evaluate". This 
 * continues until none of the new estimates are better than the base. This is
 * a gradient search for a minimum in mc-space.
 * 
 * To compile:
 *   cc -o lr_coursework lr_coursework.c -lm
 * 
 * To run:
 *   ./lr_coursework
 * 
 * Dr Kevan Buckley, University of Wolverhampton, 2018
 *****************************************************************************/

typedef struct point_t {
  double x;
  double y;
} point_t;

int n_data = 1000;
point_t data[];

double threadretval1;
double threadretval2;

double residual_error(double x, double y, double m, double c) {
  double e = (m * x) + c - y;
  return e * e;
}

double rms_error1(double m, double c) {
  int i;
  double mean;
  double error_sum = 0;
  
  for(i=0; i<n_data/2; i++) {
    error_sum += residual_error(data[i].x, data[i].y, m, c);  
  }
  return error_sum;
  //mean = error_sum / n_data;
  //return sqrt(mean);
}
double rms_error2(double m, double c) {
  int i;
  double mean;
  double error_sum = 0;
  
  for(i=n_data/2; i<n_data; i++) {
    error_sum += residual_error(data[i].x, data[i].y, m, c);  
  }
  return error_sum;
  //mean = error_sum / n_data;
  //return sqrt(mean);
}

 void * thread_1(void *arg){
	double *params = (double *)arg;
	threadretval1  = rms_error1(params[0], params[1]);
}

 void * thread_2(void *arg){
	double *params = (double *)arg;
	threadretval2  = rms_error2(params[0], params[1]);
}

double rms_error(double m, double c) {
  double params1[2], params2[2];
  pthread_t pt1, pt2;
  double mean;
  double error_sum = 0;

  params1[0] = m;
  params1[1] = c;
  params2[0] = m;
  params2[1] = c;
  
  pthread_create(&pt1, NULL, &thread_1, params1);
  pthread_create(&pt2, NULL, &thread_2, params2);
  pthread_join(pt1,NULL);
  pthread_join(pt2,NULL);
  
  error_sum = threadretval1 + threadretval2;
  mean = error_sum / n_data;
  return sqrt(mean);
}


  
int main() {
  int i;
  double bm = 1.3;
  double bc = 10;
  double be;
  double dm[8];
  double dc[8];
  double e[8];
  double step = 0.01;
  double best_error = 999999999;
  int best_error_i;
  int minimum_found = 0;
  
  double om[] = {0,1,1, 1, 0,-1,-1,-1};
  double oc[] = {1,1,0,-1,-1,-1, 0, 1};

  be = rms_error(bm, bc);

  while(!minimum_found) {
    for(i=0;i<8;i++) {
      dm[i] = bm + (om[i] * step);
      dc[i] = bc + (oc[i] * step);    
    }
      
    for(i=0;i<8;i++) {
      e[i] = rms_error(dm[i], dc[i]);
      if(e[i] < best_error) {
        best_error = e[i];
        best_error_i = i;
      }
    }

    printf("best m,c is %lf,%lf with error %lf in direction %d\n", 
      dm[best_error_i], dc[best_error_i], best_error, best_error_i);
    if(best_error < be) {
      be = best_error;
      bm = dm[best_error_i];
      bc = dc[best_error_i];
    } else {
      minimum_found = 1;
    }
  }
  printf("minimum m,c is %lf,%lf with error %lf\n", bm, bc, be);

  return 0;
}

point_t data[] = {
  {72.63,111.69},{73.20,99.89},{90.46,125.35},{65.35,80.35},
  {82.37,110.58},{65.24,97.75},{72.81,115.34},{77.47,94.48},
  {69.55,80.26},{68.65,90.14},{53.07,93.67},{ 0.99,32.84},
  {77.66,115.51},{27.91,49.88},{26.35,65.35},{84.38,81.38},
  {89.94,106.21},{48.33,67.93},{78.89,116.71},{57.34,88.91},
  {33.23,64.44},{79.42,104.77},{52.57,68.26},{42.58,74.58},
  {10.54,36.99},{66.94,93.73},{63.09,92.96},{ 0.08,30.73},
  {66.00,103.68},{ 0.23,10.76},{99.03,126.82},{ 4.72,38.96},
  {26.84,39.84},{ 5.89,49.53},{ 8.82,40.17},{32.36,57.31},
  {77.77,90.36},{ 7.35,22.08},{62.46,100.87},{73.44,95.14},
  {13.69,31.41},{47.16,67.01},{63.76,79.36},{32.38,58.67},
  {33.29,61.30},{68.21,92.07},{ 5.23,14.39},{23.18,44.47},
  {16.52,51.53},{49.48,90.63},{ 1.33,12.69},{49.88,80.13},
  {55.16,84.09},{27.50,51.88},{12.86,40.75},{86.93,122.19},
  {40.61,63.91},{72.30,100.09},{22.57,48.95},{ 3.42,22.55},
  {76.75,92.67},{14.21,48.59},{20.05,62.97},{47.80,65.56},
  {94.64,114.31},{98.52,130.26},{25.35,33.42},{36.27,63.51},
  { 2.25,12.65},{58.29,87.37},{41.33,66.86},{22.45,28.58},
  {26.18,45.08},{87.26,124.53},{21.41,26.78},{89.88,108.49},
  {83.66,112.88},{10.99,28.14},{44.71,60.20},{29.14,55.01},
  {28.41,52.44},{20.30,61.29},{49.86,80.98},{56.16,100.15},
  {11.89,42.27},{33.77,57.52},{14.01,25.38},{47.97,64.96},
  {72.72,105.16},{47.50,72.49},{29.81,62.18},{57.59,80.89},
  {14.73,33.04},{23.89,51.46},{47.20,77.49},{90.57,124.65},
  {41.92,71.99},{37.12,70.22},{83.68,112.77},{82.78,100.46},
  {61.50,87.29},{56.55,81.29},{53.67,76.59},{56.57,80.54},
  {31.55,38.12},{19.90,34.79},{17.66,34.92},{48.25,69.59},
  { 3.06,18.89},{ 9.85,26.38},{94.98,118.97},{30.14,56.83},
  { 1.81, 3.15},{97.09,128.66},{78.48,104.02},{86.33,117.02},
  {20.81,28.39},{84.19,110.28},{16.16,17.66},{36.42,58.77},
  {42.57,69.69},{15.19,29.84},{13.20,33.90},{16.77,27.39},
  { 9.67,29.72},{64.57,76.41},{60.24,81.88},{18.79,17.22},
  {97.38,128.53},{56.13,82.16},{84.24,108.93},{32.10,28.99},
  {57.23,86.69},{76.34,108.26},{83.64,106.65},{20.84,23.64},
  {57.01,86.10},{53.19,82.88},{97.71,122.92},{15.15,52.20},
  {55.22,88.50},{39.20,51.15},{90.02,110.80},{93.90,150.87},
  {28.81,59.60},{95.78,117.87},{60.29,99.08},{77.90,102.61},
  {38.81,61.42},{73.46,104.02},{ 9.18,59.35},{ 6.06,20.48},
  {73.15,99.53},{97.09,130.90},{76.58,101.71},{64.61,120.89},
  {16.24,46.76},{70.98,81.89},{98.04,128.27},{22.24,50.80},
  {97.35,133.02},{ 0.16,27.12},{54.66,93.04},{39.63,56.38},
  {96.34,137.84},{27.19,47.72},{92.27,120.60},{64.85,90.91},
  {68.16,85.60},{57.33,86.58},{ 2.54,31.94},{52.20,66.93},
  { 3.07,14.46},{40.75,72.34},{59.10,87.91},{53.60,81.14},
  {12.99,39.94},{ 8.53,19.38},{97.99,127.41},{10.58,34.13},
  {56.63,73.28},{85.49,115.77},{51.37,80.83},{38.66,62.36},
  {99.58,129.28},{ 2.16,23.81},{84.97,113.10},{75.88,98.25},
  {34.68,48.32},{73.52,115.65},{84.82,125.75},{61.88,105.97},
  {62.38,82.63},{17.72,40.52},{ 7.87,21.32},{48.29,73.86},
  {21.44,44.73},{56.68,82.90},{36.38,43.63},{44.79,89.12},
  {17.05,46.81},{ 9.57,30.82},{26.53,52.27},{15.72,33.72},
  {48.79,53.39},{81.75,114.53},{35.89,68.04},{46.97,61.89},
  {45.38,68.16},{ 3.19,33.89},{38.74,59.80},{32.38,42.64},
  {10.92,30.05},{32.22,62.43},{33.61,52.78},{20.02,40.72},
  { 4.24,21.96},{82.04,114.25},{97.83,130.72},{88.12,108.56},
  {32.71,65.12},{84.45,109.51},{87.87,101.42},{48.24,73.61},
  {21.40,39.96},{86.26,110.08},{ 8.94,29.61},{85.57,127.01},
  {26.64,31.71},{61.97,84.61},{77.30,118.80},{69.62,91.28},
  {74.40,126.99},{78.70,111.01},{32.89,54.74},{ 9.26,32.69},
  {23.58,36.88},{94.91,112.34},{45.29,86.32},{43.98,66.85},
  {23.53,44.82},{91.93,106.66},{45.29,72.50},{46.77,67.78},
  {22.13,53.60},{ 6.63,24.94},{32.52,73.33},{55.09,80.41},
  {47.76,66.40},{27.73,69.66},{83.68,90.82},{50.53,77.81},
  {96.47,128.73},{ 5.19,40.07},{91.90,116.65},{88.28,121.15},
  {13.53,47.68},{85.20,127.51},{55.75,79.92},{21.98,44.18},
  {13.87,39.52},{71.85,117.68},{26.52,51.75},{21.12,37.98},
  { 9.99,27.78},{56.37,77.44},{36.01,53.34},{92.34,111.39},
  {23.36,37.77},{94.47,113.99},{11.95,39.10},{41.35,56.82},
  { 1.67, 2.19},{52.95,67.02},{60.37,90.72},{21.54,32.87},
  {65.87,100.48},{10.18,42.82},{ 3.68,22.66},{95.07,135.43},
  {95.91,115.54},{ 0.44,36.68},{33.30,66.05},{27.91,57.99},
  {79.33,116.10},{ 0.25,-3.67},{14.85,33.54},{89.37,123.59},
  {93.86,125.80},{93.66,111.26},{65.44,105.24},{23.51,34.18},
  {33.20,53.69},{45.48,70.36},{ 2.57,34.02},{94.90,124.49},
  {39.78,66.07},{37.66,49.39},{18.13,49.25},{29.97,45.60},
  { 7.24,17.28},{21.65,47.24},{53.41,69.40},{ 6.27,18.12},
  {18.40,64.89},{90.88,128.86},{46.73,66.49},{82.80,93.86},
  { 7.03,27.66},{82.47,110.05},{43.47,60.47},{79.80,117.67},
  {90.86,129.41},{37.13,59.81},{90.50,120.04},{ 0.49,18.94},
  {76.20,88.69},{67.60,100.60},{45.21,74.90},{21.04,28.32},
  {44.44,76.94},{ 9.17,29.25},{14.60,26.32},{90.86,100.86},
  {28.08,37.98},{22.76,52.51},{40.84,70.63},{55.85,78.03},
  {10.45,33.92},{49.89,59.03},{ 8.04,14.36},{41.38,71.04},
  {22.65,47.33},{ 6.92,19.19},{59.22,76.22},{19.33,33.87},
  {36.69,79.86},{28.02,57.97},{15.22,45.79},{77.57,112.63},
  {68.63,91.04},{31.17,49.75},{68.09,89.70},{29.95,63.35},
  {22.80,29.92},{ 0.06,26.15},{90.56,125.86},{58.66,89.38},
  {33.32,45.28},{48.19,81.01},{29.49,45.46},{ 4.18,20.52},
  {15.53,32.29},{47.55,70.49},{97.01,117.42},{ 3.77,38.29},
  {14.93,36.91},{41.30,64.07},{31.40,77.34},{ 7.28,17.82},
  {86.84,94.77},{ 8.05,35.48},{66.67,98.19},{45.58,79.35},
  {24.57,52.07},{ 2.20,19.95},{64.19,96.67},{64.26,76.25},
  {88.13,106.97},{97.93,140.11},{40.52,55.86},{42.84,55.77},
  {90.64,111.12},{81.17,92.49},{58.72,88.64},{43.48,61.74},
  {86.25,106.34},{48.95,71.04},{88.14,112.02},{12.00,21.09},
  {28.94,62.44},{13.99,51.67},{17.37,42.92},{88.85,111.53},
  {91.16,121.79},{78.22,80.44},{66.73,108.65},{ 1.49,14.78},
  {19.54,54.90},{29.58,54.97},{63.83,93.48},{93.39,110.57},
  { 0.47,40.67},{87.69,122.72},{ 1.26,20.04},{38.86,49.53},
  {79.32,108.35},{59.73,71.27},{35.68,56.63},{84.36,119.46},
  {83.79,100.47},{23.53,50.83},{95.80,125.21},{11.23,37.84},
  {70.69,95.09},{11.47,34.05},{87.11,116.41},{43.83,51.15},
  {96.86,133.61},{95.16,114.09},{60.23,91.82},{13.31,38.57},
  {96.47,111.65},{47.37,53.05},{23.53,40.76},{47.02,84.98},
  { 5.56,16.76},{24.41,52.34},{ 6.66, 7.02},{42.04,82.35},
  {13.71,33.98},{43.53,65.91},{86.60,110.92},{65.43,95.37},
  {52.68,81.41},{ 2.88,21.84},{64.96,99.48},{57.82,78.66},
  {21.25,58.85},{93.82,136.63},{89.82,122.17},{39.71,89.50},
  {10.51,40.34},{38.47,55.08},{92.08,125.95},{67.63,100.51},
  {42.48,56.84},{25.25,36.88},{14.64,36.36},{27.69,51.98},
  {67.14,107.99},{41.26,71.36},{14.91,27.25},{62.53,105.07},
  {58.08,82.53},{26.74,63.31},{67.93,92.53},{ 1.24,12.45},
  {37.01,58.39},{57.03,87.16},{52.36,74.11},{60.74,88.69},
  {27.10,67.38},{22.06,50.95},{69.09,90.78},{50.06,78.47},
  {30.99,61.15},{19.03,49.56},{60.31,74.78},{96.21,130.53},
  {81.17,129.09},{89.50,108.23},{75.71,91.09},{72.74,96.84},
  {81.55,118.86},{35.33,41.22},{42.46,91.67},{64.03,86.54},
  {82.43,120.28},{ 3.15,15.36},{88.86,122.59},{47.01,66.03},
  {51.01,84.76},{69.59,89.14},{46.94,83.56},{ 9.97,36.87},
  {71.81,93.01},{79.26,104.84},{57.73,78.47},{58.88,98.20},
  {71.36,96.21},{17.49,36.18},{50.92,68.89},{48.64,65.30},
  {69.34,88.10},{97.26,131.75},{51.25,85.62},{30.65,55.72},
  {82.81,91.28},{77.58,105.18},{55.51,66.68},{31.31,53.97},
  {63.11,93.86},{71.85,92.96},{22.54,55.52},{69.26,109.68},
  {54.07,73.92},{75.16,93.29},{30.69,50.81},{50.12,86.25},
  { 9.06,30.15},{ 1.25,23.80},{26.91,63.77},{ 3.60,32.43},
  { 7.92,38.60},{59.36,86.75},{34.18,66.29},{64.94,83.74},
  {40.77,67.76},{23.02,44.34},{64.71,95.69},{79.55,86.73},
  {55.61,78.24},{31.42,57.86},{33.17,62.04},{19.79,44.63},
  {43.15,71.16},{55.14,82.36},{29.41,32.88},{34.33,67.07},
  {52.05,72.33},{30.61,54.06},{16.19,44.31},{74.04,107.75},
  {45.61,68.03},{99.03,115.97},{68.71,82.36},{24.10,46.59},
  {34.50,73.46},{56.48,81.65},{40.13,66.19},{14.08,38.48},
  { 3.97,15.83},{27.27,55.54},{65.79,111.13},{ 7.89,28.74},
  { 2.51,32.15},{78.89,106.91},{66.85,74.39},{96.52,123.77},
  {70.52,111.31},{75.60,87.75},{68.79,103.36},{11.44,40.62},
  {19.92,50.02},{68.32,100.47},{10.61,39.53},{83.05,97.01},
  { 6.55,31.51},{23.37,51.64},{52.12,85.93},{24.12,35.01},
  {71.52,97.97},{17.23,33.84},{70.24,80.92},{84.59,130.18},
  {26.66,50.01},{13.02,29.60},{82.42,120.10},{ 5.53,28.76},
  {34.44,68.11},{88.63,124.64},{90.03,106.85},{ 3.19,38.84},
  {42.00,83.63},{48.84,67.02},{45.93,75.84},{11.64,25.38},
  {26.39,50.62},{ 5.45,23.47},{85.28,119.98},{18.22,36.40},
  {28.33,61.70},{51.51,75.18},{68.07,97.91},{35.72,60.09},
  {27.29,53.67},{19.94,37.33},{57.20,84.81},{44.16,65.45},
  {23.90,26.20},{44.55,57.02},{60.68,91.52},{70.94,97.32},
  {20.89,54.01},{ 7.51,19.24},{49.54,69.79},{17.66,44.85},
  {46.68,70.90},{35.68,44.05},{78.30,101.53},{39.99,60.23},
  { 0.49, 8.40},{84.98,111.25},{33.05,73.49},{ 1.73,11.02},
  {16.97,41.82},{76.30,110.15},{75.79,106.11},{80.59,98.93},
  {24.06,51.35},{38.21,41.45},{88.48,121.28},{ 6.63,47.51},
  {68.26,100.83},{77.69,104.30},{63.61,96.83},{80.29,107.93},
  {72.01,93.72},{39.75,66.75},{23.01,28.19},{12.84,25.28},
  {76.15,121.93},{17.90,15.85},{20.30,28.57},{39.86,54.35},
  {60.12,78.21},{ 0.87,32.72},{58.44,89.19},{ 8.92,31.25},
  {62.99,89.42},{88.54,103.46},{91.70,124.62},{57.47,76.47},
  {77.90,109.96},{38.20,72.24},{ 9.11,33.37},{41.82,59.72},
  {68.18,93.19},{97.68,127.08},{16.00,32.66},{14.72,48.08},
  {39.99,61.89},{69.85,76.71},{68.16,99.01},{68.18,75.94},
  {79.93,106.72},{88.03,119.47},{36.93,53.59},{ 5.95,39.15},
  { 9.49,29.60},{42.01,73.66},{93.97,121.17},{21.53,46.67},
  {80.38,115.23},{30.48,70.28},{57.65,78.81},{74.72,118.18},
  {32.90,54.58},{71.17,113.31},{31.59,57.09},{33.68,47.92},
  {34.36,53.05},{67.31,81.74},{97.53,135.51},{25.92,45.38},
  { 9.66,15.17},{28.18,42.22},{24.14,30.71},{35.02,61.60},
  {12.54,17.51},{36.39,43.02},{88.84,122.13},{85.70,121.66},
  {69.42,89.05},{93.88,105.61},{31.77,72.61},{ 3.71,37.61},
  { 7.34,36.97},{67.21,115.80},{26.41,39.26},{66.38,79.06},
  {29.27,58.35},{63.94,72.42},{73.05,99.23},{15.83,23.91},
  {81.73,101.88},{88.09,111.70},{90.49,113.15},{71.23,107.03},
  {30.98,39.09},{65.73,97.44},{34.08,53.08},{42.58,63.68},
  {99.22,125.65},{ 2.50,25.05},{74.58,97.71},{54.35,83.79},
  {74.26,104.18},{68.49,85.62},{12.43,32.83},{93.10,129.57},
  {25.95,34.79},{25.22,30.94},{54.17,80.53},{69.82,93.18},
  {49.63,74.19},{27.30,32.73},{51.21,82.36},{22.84,47.08},
  {97.36,136.76},{55.99,84.76},{85.08,91.97},{56.83,64.91},
  { 9.68,31.78},{17.23,47.87},{37.46,69.34},{21.20,40.27},
  {18.01,47.93},{71.41,97.48},{75.90,111.67},{83.00,100.90},
  {35.26,54.80},{81.15,99.74},{98.19,126.60},{43.83,79.92},
  {82.48,108.41},{46.12,67.76},{79.08,115.84},{21.64,51.49},
  {60.36,87.16},{71.32,102.95},{14.03,49.97},{59.72,98.60},
  {23.71,38.76},{21.96,49.47},{84.80,104.49},{41.31,58.00},
  { 3.21,14.06},{21.58,51.92},{79.30,108.04},{18.04,34.20},
  {15.67,47.02},{68.82,95.04},{68.81,88.57},{ 0.96, 3.44},
  {10.64,13.45},{13.62,36.42},{93.22,119.60},{ 7.51,33.85},
  {18.94,34.12},{20.65,48.95},{13.83,46.90},{59.89,81.75},
  { 1.10,26.11},{11.71,34.00},{85.28,99.47},{27.36,60.78},
  {87.38,127.13},{90.21,112.58},{59.29,86.48},{57.36,82.24},
  {30.74,65.81},{38.18,58.17},{57.10,81.41},{27.88,56.75},
  {53.59,86.85},{91.00,115.68},{43.75,51.41},{33.17,63.98},
  {81.08,115.20},{47.96,45.20},{14.17,24.91},{65.02,76.14},
  {40.07,61.87},{46.60,64.35},{90.85,108.08},{61.26,92.80},
  {54.62,67.09},{82.69,114.43},{49.57,59.99},{34.50,57.62},
  {11.87,25.47},{71.27,89.13},{ 5.96,24.13},{64.39,92.08},
  {89.98,117.03},{36.66,68.26},{16.60,31.88},{63.20,88.89},
  {76.97,105.03},{29.55,47.19},{81.05,102.83},{71.99,96.48},
  {62.45,81.96},{ 6.76,34.44},{83.90,106.92},{53.52,89.96},
  { 8.55,24.59},{77.64,125.41},{ 4.80,17.33},{ 6.74,24.14},
  {91.16,118.89},{75.12,102.72},{22.88,38.98},{78.59,115.78},
  { 9.57,33.89},{76.80,112.30},{85.54,103.66},{40.30,66.57},
  { 2.22,31.48},{67.77,94.21},{79.23,108.33},{51.95,60.83},
  {83.26,112.27},{ 0.74,34.17},{76.75,103.20},{52.90,85.34},
  {95.69,116.29},{91.96,118.96},{94.81,113.23},{26.23,47.63},
  {32.26,56.24},{89.95,122.23},{63.24,105.04},{28.72,52.62},
  {14.23,36.53},{74.08,98.36},{26.46,51.15},{57.64,76.63},
  { 6.66,13.41},{53.23,70.60},{64.48,92.95},{15.79,24.52},
  {12.45,36.71},{90.57,135.22},{85.01,103.86},{35.52,51.67},
  {85.29,106.56},{72.49,91.53},{75.65,79.07},{88.54,106.96},
  {88.52,99.70},{97.58,137.13},{46.25,63.44},{ 5.20,16.23},
  {46.65,57.09},{57.18,78.38},{68.09,98.26},{71.24,113.62},
  {27.55,65.66},{75.40,115.14},{29.41,67.00},{55.87,65.39},
  { 3.39,11.63},{83.43,104.33},{98.45,130.96},{75.44,88.29},
  {57.73,73.91},{35.35,58.33},{ 5.04,29.12},{97.91,131.31},
  {95.36,125.41},{53.21,84.41},{71.29,75.61},{31.64,54.07},
  {24.36,54.72},{41.69,76.08},{13.64,24.50},{63.14,100.94},
  {32.02,45.69},{79.84,110.47},{29.49,67.58},{58.11,107.35},
  {23.78,45.59},{13.31,53.22},{ 2.91,30.04},{35.04,44.39},
  {77.05,98.95},{67.32,99.15},{ 0.10, 9.22},{68.26,106.22},
  {69.72,97.85},{93.68,154.78},{56.18,80.27},{93.80,117.83},
  {86.92,117.59},{22.18,37.39},{63.93,91.59},{ 5.68,21.95},
  {11.27,41.28},{91.39,125.11},{87.34,133.51},{53.60,73.86},
  {51.18,78.60},{77.31,100.13},{71.47,73.48},{ 1.10,27.43},
  {34.74,66.44},{70.56,99.77},{50.92,72.21},{94.71,133.66},
  { 1.64,26.64},{60.38,75.85},{ 9.82,45.71},{90.49,131.50},
  {38.82,85.17},{80.50,95.40},{94.96,121.36},{41.65,68.38},
  {80.88,130.42},{80.90,96.95},{79.37,92.52},{74.44,101.63},
  {20.76,44.22},{49.40,83.55},{77.37,114.67},{10.65,41.11},
  {28.97,41.71},{91.31,129.99},{12.71,29.11},{91.96,126.47},
  {62.84,96.15},{69.01,99.05},{32.99,49.66},{61.53,100.32},
  {87.03,128.24},{11.44,41.54},{98.12,122.81},{25.14,56.93},
  {80.30,118.37},{16.36,24.42},{92.24,126.11},{80.64,128.17},
  {27.40,45.32},{12.26,32.54},{57.45,76.05},{73.91,99.26},
  {72.44,102.42},{96.34,112.34},{86.97,112.79},{ 9.93,32.52},
  {51.28,82.59},{19.44,34.94},{80.79,88.59},{26.68,59.24},
  {89.38,113.33},{72.49,94.62},{34.91,59.26},{89.63,117.11},
  {34.27,60.78},{13.22,35.24},{17.31,31.81},{54.06,88.07},
  {73.38,99.23},{55.54,94.10},{14.15,38.31},{78.50,96.75},
  {81.68,78.99},{68.26,105.23},{52.94,66.97},{99.06,119.49},
  {98.03,131.00},{49.85,78.86},{68.08,101.81},{68.52,93.71},
  {73.84,99.07},{ 6.14, 8.37},{94.89,110.95},{98.18,134.32},
  {26.43,36.74},{29.81,50.03},{71.01,111.74},{99.20,137.96},
  {62.41,82.78},{28.58,62.11},{51.32,71.25},{93.22,137.47},
  {63.76,82.79},{83.20,116.94},{23.29,48.31},{15.96,21.68},
  { 8.55,17.66},{99.24,117.55},{55.67,80.73},{49.13,78.78},
  {55.28,82.66},{66.41,95.37},{16.82,36.99},{72.55,114.99},
  {46.92,69.92},{ 1.01,18.36},{89.34,117.55},{ 3.74, 8.05},
  {35.91,55.18},{55.64,81.69},{76.89,106.47},{48.13,75.39},
  {84.80,125.62},{28.51,64.03},{ 5.12,31.27},{13.85,58.49},
  {74.32,102.31},{43.82,68.74},{54.15,77.75},{21.88,48.35},
  {85.60,101.85},{79.92,113.48},{48.91,62.49},{35.11,64.90}
};