#include <stdio.h>
#include <graphics.h>
#include <math.h>
#include <Windows.h>

#define PI 3.1416

int *Point(float *xyz){
	static int r_xy[2];
	r_xy[0] = 100 * xyz[0] + 200;
	r_xy[1] = 200 - 100 * xyz[1] + 30 * xyz[2];
	return r_xy;
}


float *rotate(float *xyz, float angle){
	static float r_xyz[3];
	
	float x=0.5, y=0.5, z=0.5;
	
	float a = xyz[0] - x;
	float b = xyz[2] - z;
	float sint = sin(angle * PI/180);
	float cost = cos(angle * PI/180);
	r_xyz[0] = a*cost - b*sint + x;
	r_xyz[2] = a*sint + b*cost + z;
	
	r_xyz[1] = xyz[1];
	
	return r_xyz;
}


int main()
{
	int gd=DETECT, gm;
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	
	float points[8][3] = {{0,0,0}, {0,1,1}, {0,0,1}, {1,0,1}, 
						  {1,0,0}, {1,1,0}, {0,1,0}, {1,1,1}};
	int lines[12][2] = {{0,2}, {0,4}, {0,6}, {7,1}, {7,3}, {7,5},
						{1,2}, {2,3}, {3,4}, {4,5}, {5,6}, {6,1}};
						
	for (int c=0; c<1000; c++){
				
		cleardevice();
		// Rendering:			
		int *point;
		int p1x, p1y, p2x, p2y;
		for (int i=0; i<12; i++){
			point = Point(points[lines[i][0]]);
			p1x = point[0], p1y = point[1];
			point = Point(points[lines[i][1]]);
			p2x = point[0], p2y = point[1];
			line(p1x, p1y, p2x, p2y);
		}
		
		//Rotating:
		for (int i=0; i<8; i++){
			float *fpoint = rotate(points[i], 3.6);
			points[i][0] = fpoint[0];
			points[i][1] = fpoint[1];
			points[i][2] = fpoint[2];
		}
		Sleep(40);
			
	}

	getch();
}