
#include<bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include<stdio.h>
#include<semaphore.h>
#include<sys/types.h>
#include<unistd.h>
using namespace std;
//#define NUM_THREADS 5
vector<pair< char,pair<int ,int > > >defense, troops;
map<char ,int >damage, hit_points,range;
set<char >all_troops, all_defense;

int flag ;

char board[28][28];

void init_board(char board[][28])
{
	for(int i=0;i<28;i++)
	{
		for(int j=0;j<28;j++)
		{
			board[i][j]='.';
		}
	}
}

void print_board(char board[][28])
{
	for(int i=0;i<28;i++)
	{
		for(int j=0;j<28;j++)
		{
			cout<<board[i][j];
		}
		cout<<"\n";
	}
}
void *PrintHello(void *threadid) {
   long tid;
   sleep(5);
   tid = (long)threadid;
   cout << "Hello World! Thread ID, " << tid << endl;

   pthread_exit(NULL);
}

void *defense_attack(void *cu)
{
	label1:
	int cur = (long )cu;
	int mi = 1000,mi_pos=0;
	int df = defense[cur].second.first,ds = defense[cur].second.second;
	for(int i=0;i<troops.size();i++)
	{
		int tf = troops[i].second.first,ts = troops[i].second.first;
		if(abs(df-tf)+abs(ds-ts)<mi)
		{
			mi=abs(df-tf)+abs(ds-ts);
			mi_pos =i;
		}
	}

	char troop_name = troops[mi_pos].first;
	char defense_name = defense[cur].first;

	cout<<"\ndefense "<<defense_name<<" is going to attack "<<troop_name<<" troop\n";

	while(hit_points[troop_name]>0 && hit_points[defense_name]>0 )
	{
		int tf = troops[mi_pos].second.first , ts = troops[mi_pos].second.second;

		if(abs(tf-df)<=range['C'])
		{
			hit_points[troop_name] = hit_points[troop_name] - damage[defense_name];
		}
		cout<<"\nhit point of troop "<<troop_name<<" is "<<hit_points[troop_name]<<"\n\n";
		sleep(1);
	}
	//if(all_defense.size()==0 || all_troops.size()==0)
		flag=2;

	/*if(hit_points[troop_name]>0)
	{
		cout<<"\n Defense "<<defense_name<<" "<<" died in battle\n";
		//cout<<"\nyour attack wins \n";
	}
	else
	{
		if(all_troops.find(troop_name)!=all_troops.end())
		{
			all_troops.erase(all_troops.find(troop_name));
		}
		goto label1;
		//cout<<"\ndefense won\n";
	}*/

}
void *troops_attack(void *cu)
{
	label:
	int cur = (long)cu;
	int mi = 1000,mi_pos=0;
	int tf = troops[cur].second.first , ts = troops[cur].second.second;

	for(int i=0;i<defense.size();i++)
	{
		int df = defense[i].second.first,ds = defense[i].second.second;
		if(abs(tf-df)+abs(ts-ds)<mi)
		{
			mi = abs(tf-df)+abs(ts-ds);
			mi_pos = i;
		}
	}

	char troop_name = troops[cur].first;
	char defense_name = defense[mi_pos].first;

	cout<<"\ntroop "<<troop_name<<" is going to attack "<<defense_name<<" defense\n";

	int df = defense[mi_pos].second.first,ds = defense[mi_pos].second.second;

	while(hit_points[troop_name]>0 && hit_points[defense_name]>0 )
	{

		int tf = troops[cur].second.first , ts = troops[cur].second.second;

		if(abs(tf-df)<=range[troop_name] && abs(ts-ds)<=range[troop_name])
		{
			hit_points[defense_name] = hit_points[defense_name] - damage[troop_name];
		}
		else
		{
			if(abs(tf-df)>range[troop_name])
			{
				troops[cur].second.first = tf - (tf-df)/(abs(tf-df));
			}
			if(abs(ts-ds)>range[troop_name])
			{
				troops[cur].second.second = ts - (ts-ds)/(abs(ts-ds));
			}
		}

		board[tf][ts] = '.';
		tf = troops[cur].second.first ; ts = troops[cur].second.second;

		board[tf][ts] = troop_name;

		cout<<"\nhit point of defense "<<defense_name<<" is "<<hit_points[defense_name]<<"\n";
		cout<<"\n"<<" position of troop is "<<troops[cur].second.first<<" "<<troops[cur].second.second<<"\n\n";
		print_board(board);
		cout<<"\n";
		sleep(1);
	}
	//if(all_defense.size()==0 || all_troops.size()==0)
		flag=2;
	/*if(hit_points[troop_name]>0)
	{
		//cout<<"\nyour attack wins \n";
		if(all_defense.find(defense_name)!=all_defense.end())
		{
			all_defense.erase(all_defense.find(defense_name));
		}
		goto label;
	}
	else
	{
		cout<<"\n attacker "<<troop_name<<" died in battle \n";
		//cout<<"\ndefense won\n";
	}*/

}

int main () {
   //pthread_t threads[NUM_THREADS];
   int rc;
   int i;

   init_board(board);//initializing board

   //canon at (13,13)
   board[13][13] = 'C';
   damage['C'] = 5;
   hit_points['C'] = 60;
   defense.push_back({'C',{13,13}});
   range['C'] = 7;
   all_defense.insert('C');
   pthread_t thread_defense[3];

   //archer at(3,3)
   board[3][3] = 'A';
   damage['A'] = 5;
   hit_points['A'] = 50;
   troops.push_back({'A',{3,3}});
   range['A'] = 4;
   all_troops.insert('A');
   pthread_t thread_troops[3];

   board[7][7] = 'G';
   damage['G'] = 5;
   hit_points['G'] = 70;
   troops.push_back({'G',{7,7}});
   range['G'] = 1;
   all_troops.insert('G');

   print_board(board);

   for(i=0;i<troops.size();i++)
   {
   		rc=pthread_create(&thread_troops[i],NULL,troops_attack,(void*)i);
   }

   for(i=0;i<defense.size();i++)
   {
   		rc=pthread_create(&thread_defense[i],NULL,defense_attack,(void*)i);
   		//sleep(1);
   }

   for(int i = 0 ; i < 1 ; i++)
		pthread_join(thread_troops[i],NULL);
	for(int i = 0 ; i < 1 ; i++)
		pthread_join(thread_defense[i],NULL);

}
