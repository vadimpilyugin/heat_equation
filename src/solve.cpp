#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include "io.h"
#include "params.h"

int stoi(const char *s)
{
	stringstream tmp(s);
	int a;
	tmp >> a;
	return a;
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Wrong arguments! Usage:\n");
		fprintf(stderr, "\tbuild/bin/solve <output_file>\n");
		throw 1;
	}
	
	// Загрузка данных
	ofstream output(argv[1], ios_base::trunc);
	vector<float> input(x_steps);
	float dt = (time_top - time_bottom)/(time_steps - 1);
	float dx = (x_right - x_left)/(x_steps - 1);
	for(int i = 0; i < x_steps; i++)
		input[i] = start_conditions(x_left + (i-1)*dx);
	
	// Решение
	if(method == 1)
	{
		float C = 2*D*dt/(dx*dx);
		if(C>1)
		{
			printf("dt = %f, dx = %f, C = %f\n", dt, dx, C);
			printf("Решение неустойчиво! Уменьшите шаг по времени или увеличьте по пространству!\n");
			exit(1);
		}
		vector<float> last(x_steps), next(x_steps);
		last = input;
		for(int n = 2; n < time_steps; n++)
		{
			for(int i = 1; i < x_steps-1; i++)
				next[i] = C/2*(last[i-1]-2*last[i]+last[i+1]) + dt*source_func(x_left + (n-1)*dx, time_bottom + (n-1)*dt) + last[i];
			if(border_type == 1)
			{
				next[0] = left_border(time_bottom + (n-1)*dt);
				next[x_steps-1] = right_border(time_bottom + (n-1)*dt);
			}
			else if(border_type == 2)
			{
				next[0] = next[1]-dx*left_border(time_bottom + (n-1)*dt);
				next[x_steps-1] = next[x_steps-2]+dx*right_border(time_bottom + (n-1)*dt);
			}
			put_line(next, output);
			last=next;
		}
	}
	else if(method == 2)
	{
		//TODO: implement
	}
}
