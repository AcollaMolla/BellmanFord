#include "prog3.h"

#define THIS_NODE 1
#define INFINITY 999

extern int TRACE;
extern int YES;
extern int NO;



struct distance_table 
{
  int costs[4][4];
} dt1;
struct rtpkt pkt1;

/* students to write the following two routines, and maybe some others */


void rtinit1()
{
	int i,j;
	//Initialize and print routing table
	for (i=0; i<4; i++)	
		for (j=0; j<4; j++)
			dt1.costs[i][j]=INFINITY;
	dt1.costs[1][0] = 1;
	dt1.costs[1][1] = 0;
	dt1.costs[1][2] = 1;
	dt1.costs[1][3] = INFINITY;
	printdt1(&dt1);
	
	//Create packet
	create_pkt1();
	
	//Send this routing table to all neighbours
	send_pkt1();
}


void rtupdate1(struct rtpkt *rcvdpkt)
{
	int i,j;
	int source = rcvdpkt->sourceid, change;
	
	//Determine if the incoming distances are shorter than our current
	for(i=0;i<4;i++){
		if(rcvdpkt->mincost[i] < dt1.costs[source][i]){
			dt1.costs[source][i] = rcvdpkt->mincost[i];
			change = 1;
		}
	}
	
	for(i=0;i<4;i++){
		if(rcvdpkt->mincost[i] + dt1.costs[THIS_NODE][source] < dt1.costs[THIS_NODE][i]){
			dt1.costs[THIS_NODE][i] = rcvdpkt->mincost[i] + dt1.costs[THIS_NODE][source];
			change = 1;
		}
	}
	printf("-------------The updated routing table:-------------\n");
	printdt1(&dt1);
	
	//Create a new packet
	create_pkt1();
	
	//Send updated packet
	if(change)send_pkt1();
	else printf("[!] Min values hasn't been changed\n\n");
}


void printdt1(struct distance_table *dtptr)
{
  printf("                via     \n");
  printf("   D1 | 0      1      2     3 \n");
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



void linkhandler1(int linkid, int newcost)

/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}

void create_pkt1()
{
	pkt1.sourceid = THIS_NODE;
	pkt1.mincost[0] = dt1.costs[THIS_NODE][0];//Cost from node 1 to node 0
	pkt1.mincost[1] = dt1.costs[THIS_NODE][1];//Cost from node 2 to node 1
	pkt1.mincost[2] = dt1.costs[THIS_NODE][2];//Cost from node 3 to node 2
	pkt1.mincost[3] = dt1.costs[THIS_NODE][3];//Cost from node 4 to node 3
}

void send_pkt1()
{
	int i;
	for(i=0;i<4;i++){
		if(i == THIS_NODE)continue;
		pkt1.destid = i;
		tolayer2(pkt1);
	}
	printf("I've sent my routing table to %d nodes!\n", i-1);
}
