#include "prog3.h"

#define THIS_NODE 3
#define INFINITY 999

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt3;
struct rtpkt pkt3;
/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
	int i,j;
	//Initialize and print routing table
	for (i=0; i<4; i++)	
		for (j=0; j<4; j++)
			dt3.costs[i][j]=INFINITY;
	dt3.costs[THIS_NODE][0] = 7;
	dt3.costs[THIS_NODE][1] = INFINITY;
	dt3.costs[THIS_NODE][2] = 2;
	dt3.costs[THIS_NODE][3] = 0;
	printdt3(&dt3);
	
	//Create packet
	create_pkt3();
	
	//Send this routing table to all neighbours
	send_pkt3();
}


void rtupdate3(struct rtpkt *rcvdpkt)
{
	int i,j;
	int source = rcvdpkt->sourceid, change=0;
	printf("Someone is trying to reach me...@%d\n",THIS_NODE);
	
	//Replace this nodes cure
	//Determine if the incoming distances are shorter than our current
	for(i=0;i<4;i++){
		if(rcvdpkt->mincost[i] < dt3.costs[source][i]){
			dt3.costs[source][i] = rcvdpkt->mincost[i];
			change = 1;
		}
	}
	
	for(i=0;i<4;i++){
		if(rcvdpkt->mincost[i] + dt3.costs[THIS_NODE][source] < dt3.costs[THIS_NODE][i]){
			dt3.costs[THIS_NODE][i] = rcvdpkt->mincost[i] + dt3.costs[THIS_NODE][source];
			change = 1;
		}
	}
	printf("-------------The updated routing table:-------------\n");
	printdt3(&dt3);
	
	//Create a new packet
	create_pkt3();
	
	//Send updated packet
	if(change)send_pkt3();
	else printf("[!] Min values hasn't been changed\n\n");
}


void printdt3(struct distance_table *dtptr)
{
  printf("                via     \n");
  printf("   D3 | 0      1      2     3 \n");
  printf("  ----|-------------------------\n");
  printf("     0| %3d   %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][2], dtptr->costs[0][3]);
  printf("     1| %3d   %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][2], dtptr->costs[1][3]);
  printf("dest 2| %3d   %3d   %3d   %3d\n",dtptr->costs[2][0],
	 dtptr->costs[2][1],dtptr->costs[2][2], dtptr->costs[2][3]);
  printf("     3| %3d   %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][2], dtptr->costs[3][3]);
printf("----------------------------------------------------\n\n");
}


void create_pkt3()
{
	pkt3.sourceid = THIS_NODE;
	pkt3.mincost[0] = dt3.costs[THIS_NODE][0];//Cost from node 1 to node 0
	pkt3.mincost[1] = dt3.costs[THIS_NODE][1];//Cost from node 2 to node 1
	pkt3.mincost[2] = dt3.costs[THIS_NODE][2];//Cost from node 3 to node 2
	pkt3.mincost[3] = dt3.costs[THIS_NODE][3];//Cost from node 4 to node 3
}

void send_pkt3()
{
	int i;
	for(i=0;i<4;i++){
		if(i == THIS_NODE)continue;
		pkt3.destid = i;
		tolayer2(pkt3);
	}
	printf("I've sent my routing table to %d nodes!\n", i-1);
}




