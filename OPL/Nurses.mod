/*********************************************
 * OPL 12.7.0.0 Model
 * Creation Date: 27-Nov-2017 at 10:29:36 pm
 *********************************************/

int nNurses=...;
int total_hours=...;
int minHours=...;
int maxHours=...;
int maxConsec=...;
int maxPresence=...;

range N=1..nNurses;
range H=1..total_hours;
range T=3..maxPresence;

int demand_h[h in H]=...;

dvar boolean x_nh[n in N, h in H];
dvar boolean Working_Nurse[n in N];
dvar int z;

// Objective
minimize z;

subject to{

// Constraint 1
//working for a minimum number of hours
forall(n in N)
    sum(h in H) x_nh[n][h]>=minHours * Working_Nurse[n];
// Constraint 2
//working for a maximum number of hours
forall(n in N)
    sum(h in H) x_nh[n][h]<=maxHours;    
// Constraint 3
//no consecutive resting hour
forall( n in N, h in H){
  forall(t in h+3..maxPresence:h+t<=total_hours){
    3-(sum(i in (h..(h+t)) )x_nh[n][i]) + 10*(x_nh[n][h]+x_nh[n][h+t]-2) <= 0;
 }
}    
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
    sum(n in N) x_nh[n][h]>=demand_h[h];    
// Nurse Working or Not
forall(n in N)
  25*Working_Nurse[n] >=sum(h in H)x_nh[n][h];
  
// Sum of Working Nurses  
 z== sum(n in N )Working_Nurse[n]; 

}
