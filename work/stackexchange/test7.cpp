#include <iostream>
 #include <cmath>
 #include <cstdlib>
 #define DIM 1024
 #define DM1 (DIM-1)
 #define _sq(x) ((x)*(x)) // square
 #define _cb(x) abs((x)*(x)*(x)) // absolute value of cube
 #define _cr(x) (unsigned char)(pow((x),1.0/3.0)) // cube root
  
 unsigned char GR(int,int);
 unsigned char BL(int,int);
  
 unsigned char RD(int i,int j){
    // YOUR CODE HERE
#define A float a=0,b,k,r,x
#define B int e,o
#define C(x) x>255?255:x
#define R return
#define D DIM
	  R BL(i,j)*(D-i)/D;
 }
 unsigned char GR(int i,int j){
    // YOUR CODE HERE
#define E DM1
#define F static float
#define G for(
#define H r=a*1.6/D+2.4;x=1.0001*b/D
    R BL(i,j)*(D-j/2)/D;
}
 unsigned char BL(int i,int j){
    // YOUR CODE HERE
	F c[D][D];
	if(i+j<1)
	{
		A;B;G;a<D;a+=0.1)
		{
			G b=0;b<D;b++){H;G k=0;k<D;k++){x=r*x*(1-x);if(k>D/2){e=a;o=(E*x);c[e][o]+=0.01;}}}}}R C(c[j][i])*i/D;
 }
  
 void pixel_write(int,int);
 FILE *fp;
 int main(){
     fp = fopen("MathPic.ppm","wb");
     fprintf(fp, "P6\n%d %d\n255\n", DIM, DIM);
     for(int j=0;j<DIM;j++)
         for(int i=0;i<DIM;i++)
             pixel_write(i,j);
     fclose(fp);
     return 0;
 }
 void pixel_write(int i, int j){
     static unsigned char color[3];
     color[0] = RD(i,j)&255;
     color[1] = GR(i,j)&255;
     color[2] = BL(i,j)&255;
     fwrite(color, 1, 3, fp);
 }
