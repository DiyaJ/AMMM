/*********************************************
 * OPL 12.5.1.0 Model
 * Author: asaf.beduhe
 * Author: marc.josep
 * Creation Date: 16/10/2017 at 10:39:16
 *********************************************/

int nNurses=...;
int nHours=...;

int minHours=...;
int maxHours=...;
int maxConsec=...;
int maxPresence=...;

range N=1..nNurses;
range H=1..nHours;

int demand[h in H]=...;


dvar boolean w[n in N, h in H];
dvar boolean nw[n in N];


/*------------------------------
 pre-processing block 
 -------------------------------*/
execute {
	
  
};

/*------------------------------
 Objective 
 -------------------------------*/

minimize sum(n in N) nw[n];
subject to{

  // At each hour h there must be at least demand_h nurses working
  forall(h in H)
    sum(n in N) w[n,h] >= demand[h];
  
  // Each nurse must work at least minHours
  forall (n in N)  
  	sum(h in H) w[n,h] >= minHours * nw[n]; 
  	
  // Each nurse must work at most maxHours
  forall (n in N)  
  	sum(h in H) w[n,h] <= maxHours; 
  
  // Each nurse must work at most maxConsec conscutive hourse
  forall (n in N, h in H){
  	if(h < nHours - maxConsec){
  	  	sum(i in 0..(maxConsec)) w[n,h+i] <= maxConsec;
  	}  
  }
  
  //Each nurse must stay at work at most maxPresence hours at the hospital
  forall (n in N, h1 in H){
  	forall(h2 in (h1+1)..nHours){
  		w[n,h2]*h2 - w[n,h1]*h1 <= (1 - w[n,h1])*(nHours+1) + maxPresence-1;
  	}  
  }    
  
  // A nurse can rest at most one consecutive hour
  forall(n in N, h1 in H){
    forall(h2 in (h1+2)..nHours){
  		h2 - h1 <= 2 + (nHours-2)*((sum(h in (h1+1)..(h2-1))w[n,h])	+ (1- w[n,h1]) + (1 - w[n,h2]));		
  	}
  }
  	
  // wn must represent wether a nurse work or not
  forall (n in N, h in H) 
  	nw[n] >= w[n,h];  

}

/*------------------------------
 post-processing block 
 -------------------------------*/
execute {
	var total = 0;
	for(var n=1; n<=nNurses; ++n)
		total+=nw[n];
  
  	writeln("Total nurses used: " + total + ".");
};
