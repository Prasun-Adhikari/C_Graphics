#include <stdio.h>
#include <graphics.h>
#include <math.h>
#include <Windows.h>

#define PI 3.1416
#define WIREFRAME 0
#define SHADED 1

const int ROT_ANG = 5;
const int PAUSE_T = 100;
const int FRAMES = 10000;
//const int C_MODE = WIREFRAME;
const int C_MODE = SHADED;

typedef struct{
	float x, y, z;
} Point;

typedef struct{
	Point points[2];
} Line;

typedef struct{
//	Line lines[4];
	Point points[4];
} Face;


void renderlines(Line line);
void renderfaces(Face face);
int *flatten(Point point);
Point rotate(Point point, float angle);

int main()
{
	Face faces[6];
	Line lines[12];
	Point points[8];
	
	float point_coords[8][3] = {{0,0,0}, {0,1,1}, {0,0,1}, {1,0,1}, 
						  		{1,0,0}, {1,1,0}, {0,1,0}, {1,1,1}};
	int line_points[12][2] = {{0,2}, {0,4}, {0,6}, {7,1}, {7,3}, {7,5},
							  {1,2}, {2,3}, {3,4}, {4,5}, {5,6}, {6,1}};
	int face_points[6][4] = {{1,2,3,7}, {3,4,5,7}, {0,2,3,4}, 
							 {0,2,1,6}, {1,6,5,7}, {0,6,5,4}};
							 
							 
	for (int pi=0; pi<8; pi++){
		points[pi].x = point_coords[pi][0];
		points[pi].y = point_coords[pi][1];
		points[pi].z = point_coords[pi][2];
	}
	
	
	for (int fi=0; fi<6; fi++){
		for (int pi=0; pi<4; pi++){
			faces[fi].points[pi] = points[face_points[fi][pi]];
		}
	}
	
	for (int li=0; li<12; li++){
		for (int pi=0; pi<2; pi++){
			lines[li].points[pi] = points[line_points[li][pi]];
		}
	}
		
	
	int gd=DETECT, gm;
	initgraph(&gd, &gm, "C:\\tc\\bgi");

	for (int c = 0; c < FRAMES; c++){
		cleardevice();
		
		if (C_MODE == WIREFRAME){
			//Render and update lines:
			for (int li=0; li<12; li++){
				renderlines(lines[li]);
				for (int pi=0; pi<2; pi++)
					lines[li].points[pi] = rotate(lines[li].points[pi], ROT_ANG);
			}
		}
		
		else {
			//Render and update faces:
			for (int fi=0; fi<6; fi++){
				renderfaces(faces[fi]);
				for (int pi=0; pi<4; pi++)
					faces[fi].points[pi] = rotate(faces[fi].points[pi], ROT_ANG);
			}
		}
				
		Sleep(PAUSE_T);
	}
	getch();
}



int *flatten(Point point){
	static int r_xy[2];
	r_xy[0] = 100 * point.x + 200;
	r_xy[1] = 200 - 100 * point.y + 30 * point.z;
	return r_xy;
}


void renderlines(Line line_){
	int *point;
	point = flatten(line_.points[0]);
	int p1x = point[0], p1y = point[1];
	point = flatten(line_.points[1]);
	int p2x = point[0], p2y = point[1];
	line(p1x, p1y, p2x, p2y);
}


void renderfaces(Face face){
	int xy_points[10];
	for (int i=0; i<5; i++){
		int *point = flatten(face.points[i%4]);
		xy_points[2*i] = point[0];
		xy_points[2*i+1] = point[1];
	} 
	
	float l_dir[3] = {0.0, 1.2, -1.0};
	float l_dir_m = pow(pow(l_dir[0], 2) + pow(l_dir[1], 2) + pow(l_dir[2], 2), 0.5);
	
	Point *points = face.points;
	float x1 = points[1].x - points[0].x;
	float y1 = points[1].y - points[0].y;
	float z1 = points[1].z - points[0].z;
	float x2 = points[3].x - points[0].x;
	float y2 = points[3].y - points[0].y;
	float z2 = points[3].z - points[0].z;
	
	float m1 = pow(pow(x1, 2) + pow(y1, 2) + pow(z1, 2), 0.5);
	float m2 = pow(pow(x2, 2) + pow(y2, 2) + pow(z2, 2), 0.5);
	
	float vx = (y1*z2 - y2*z1) / (m1 * m2);
	float vy = (x1*z2 - x2*z1) / (m1 * m2);
	float vz = (x1*y2 - x2*y1) / (m1 * m2); 
	
	float luminance = (vx*l_dir[0] + vy*l_dir[1] + vz*l_dir[2]);
	
	int brightness =  int(abs(luminance / l_dir_m * 12));
	
	int shady_colors[12][2] = {{0,0}, {8,10}, {8,11}, {7,10}, {15,10}, {15,11}, 
							   {8,9}, {7,9}, {8,1}, {15,9}, {7,1}, {15,1}};
	
	if (vz >= -0.01 && vy >= -0.01){				   
		setcolor(DARKGRAY);
		setfillstyle(shady_colors[brightness][1], shady_colors[brightness][0]);
		fillpoly(5, xy_points);
	}
}



Point rotate(Point point, float angle){
	Point r_point;
	
	float x=0.5, y=0.5, z=0.5;
	
	float a = point.x - x;
	float b = point.z - z;
	float sint = sin(angle * PI/180);
	float cost = cos(angle * PI/180);
	
	r_point.x = a*cost - b*sint + x;
	r_point.y = point.y;
	r_point.z = a*sint + b*cost + z;
	
	return r_point;
}
