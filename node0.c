#include "prog3.h"

#define THIS_NODE 0
#define INFINITY 999

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt0;
struct rtpkt pkt0;

/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
	int i,j;
	
	//Initialize and print routing table
	for (i=1; i<4; i++)	
		for (j=0; j<4; j++)
			dt0.costs[i][j]=INFINITY;
	dt0.costs[THIS_NODE][0] = 0;
	dt0.costs[THIS_NODE][1] = 1;
	dt0.costs[THIS_NODE][2] = 3;
	dt0.costs[THIS_NODE][3] = 7;
	printdt0(&dt0);
	
	//Create packet
	create_pkt0();
	
	//Send this routing table to all neighbours
	send_pkt0();
}



void rtupdate0(struct rtpkt *rcvdpkt) 
{
	int i,j;
	int source = rcvdpkt->sourceid, change=0;
	
	//Determine if the incoming distances are shorter than our current
	for(i=0;i<4;i++){
		if(rcvdpkt->mincost[i] < dt0.costs[source][i]){
			dt0.costs[source][i] = rcvdpkt->mincost[i];
			change = 1;
		}
	}
	
	for(i=0;i<4;i++){
		if(rcvdpkt->mincost[i] + dt0.costs[THIS_NODE][source] < dt0.costs[THIS_NODE][i]){
			dt0.costs[THIS_NODE][i] = rcvdpkt->mincost[i] + dt0.costs[THIS_NODE][source];
			change = 1;
		}
	}
	printf("-------------The updated routing table:-------------\n");
	printdt0(&dt0);
	
	
	//Create a new packet
	create_pkt0();
	
	//Send updated packet
	if(change)send_pkt0();
	else printf("[!] Min values hasn't been changed\n\n");
}


void printdt0(struct distance_table *dtptr)
{
  printf("                via     \n");
  printf("   D0 | 0      1      2     3 \n");
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

void linkhandler0(int linkid, int newcost)


/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}

void create_pkt0()
{
	pkt0.sourceid = THIS_NODE;
	pkt0.mincost[0] = dt0.costs[THIS_NODE][0];//Cost from node 0 to node 0
	pkt0.mincost[1] = dt0.costs[THIS_NODE][1];//Cost from node 0 to node 1
	pkt0.mincost[2] = dt0.costs[THIS_NODE][2];//Cost from node 0 to node 2
	pkt0.mincost[3] = dt0.costs[THIS_NODE][3];//Cost from node 0 to node 3
}

void send_pkt0()
{
	int i;
		for(i=0;i<4;i++){
		if(i == THIS_NODE)continue;
		pkt0.destid = i;
		tolayer2(pkt0);
	}
	//printf("I've sent my routing table to %d nodes!\n", i-1);
}
