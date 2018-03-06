#include "prog3.h"

#define THIS_NODE 2
#define INFINITY 999

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt2;
struct rtpkt pkt2;

/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
	int i,j;
	//Initialize and print routing table
	for (i=0; i<4; i++)	
		for (j=0; j<4; j++)
			dt2.costs[i][j]=INFINITY;
	dt2.costs[THIS_NODE][0] = 3;
	dt2.costs[THIS_NODE][1] = 1;
	dt2.costs[THIS_NODE][2] = 0;
	dt2.costs[THIS_NODE][3] = 2;
	printdt2(&dt2);
	
	//Create packet
	create_pkt2();
	
	//Send this routing table to all neighbours
	send_pkt2();
}


void rtupdate2(struct rtpkt *rcvdpkt) 
{
	int i,j;
	int source = rcvdpkt->sourceid, change=0;
	printf("Someone is trying to reach me...@%d\n",THIS_NODE);
	
	//Replace this nodes cure
	//Determine if the incoming distances are shorter than our current
	for(i=0;i<4;i++){
		if(rcvdpkt->mincost[i] < dt2.costs[source][i]){
			dt2.costs[source][i] = rcvdpkt->mincost[i];
			change = 1;
		}
	}
	
	for(i=0;i<4;i++){
		if(rcvdpkt->mincost[i] + dt2.costs[THIS_NODE][source] < dt2.costs[THIS_NODE][i]){
			dt2.costs[THIS_NODE][i] = rcvdpkt->mincost[i] + dt2.costs[THIS_NODE][source];
			change = 1;
		}
	}
	printf("-------------The updated routing table:-------------\n");
	printdt2(&dt2);
	
	//Create a new packet
	create_pkt2();
	
	//Send updated packet
	if(change)send_pkt2();
	else printf("[!] Min values hasn't been changed\n\n");
}


void printdt2(struct distance_table *dtptr)
{
  printf("                via     \n");
  printf("   D2 | 0      1      2     3 \n");
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

void create_pkt2()
{
	pkt2.sourceid = THIS_NODE;
	pkt2.mincost[0] = dt2.costs[THIS_NODE][0];//Cost from node 1 to node 0
	pkt2.mincost[1] = dt2.costs[THIS_NODE][1];//Cost from node 2 to node 1
	pkt2.mincost[2] = dt2.costs[THIS_NODE][2];//Cost from node 3 to node 2
	pkt2.mincost[3] = dt2.costs[THIS_NODE][3];//Cost from node 4 to node 3
}

void send_pkt2()
{
	int i;
	for(i=0;i<4;i++){
		if(i == THIS_NODE)continue;
		pkt2.destid = i;
		tolayer2(pkt2);
	}
	printf("I've sent my routing table to %d nodes!\n", i-1);
}



