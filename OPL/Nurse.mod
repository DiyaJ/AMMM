/*********************************************
 * OPL 12.6.0.0 Model
 * Author: Diya Joseph
 * Creation Date: 29-Nov-2017 at 2:55:10 PM
 *********************************************/
/*********************************************
 * OPL 12.7.0.0 Model
 * Author: Diya Joseph
 * Creation Date: 27-Nov-2017 at 10:29:36 pm
 *********************************************/

int demand_h=...;
int nNurses=...;
int total_hours=...;
int minHours=...;
int maxHours=...;
int maxConsec=...;
int maxPresence=...;

range N=1..nNurses;
range H=1..total_hours;
range T=3..total_hours-1;

dvar boolean x_nh[n in N, h in H];
dvar boolean Working_n[n in N];
dvar int z;
/*execute {
var totalLoad=0;
for (var h=1;h<=nThreads;h++)
totalLoad += rh[h];
writeln("Total load "+ totalLoad);

};
*/

// Objective
minimize z;

subject to{

// Constraint 1
//working for a minimum number of hours
forall(n in N)
    sum(h in H) x_nh[n][h]>=minHours*Working_n[n];
// Constraint 2
//working for a maximum number of hours
forall(n in N)
    sum(h in H) x_nh[n][h]<=maxHours;    
// Constraint 3
//no consecutive resting hour
forall( n in N, h in H,t in T: h+t <= total_hours)
    3-(sum(i in (h..(h+t)) )x_nh[n][i]) + 10*(x_nh[n][h]+x_nh[n][h+t]-2) <= 0;
    
// Constraint 4
//consecutive working hours not more than maxConsec
forall( n in N, h in H)
    sum(i in (h..(h+maxConsec)) : (h + maxConsec <= total_hours))x_nh[n][i]<=maxConsec;
    
// Constraint 5
// maxPresence at hospital 
forall( n in N, h in H)
    sum(i in ((h+maxPresence)..total_hours) )x_nh[n][i]<=(1-x_nh[n][h])* maxHours;
            
// Constraint 6
//demand_h of nurses per hour
forall(h in H)
    sum(n in N) x_nh[n][h]>=demand_h;    
// Constraint 7
//Sum_n
forall(n in N)
(24*Working_n[n])>= sum(h in H)x_nh[n][h];
//Constraint 8
//z
z >= sum(n in N)Working_n[n];

}

/*execute {

for (var c=1;c<=nCPUs;c++) {
var load=0;
for(var t=1;t<=nTasks;t++){
for (var h=1;h<=nThreads;h++)
{load+=(rh[h]* x_tc[t][c]*TH[t][h]);
// writeln("CPU " + c + " loaded at " + load + " "+ h);
//writeln("CPU " + c + " loaded at t: "+ t + " h:"+ h + " "+ load + "%");
}
}
load = (1/(3*rc[c]))*load;
writeln("CPU " + c + " loaded at " + load + "%");
}
};
*/ 