 
  
#include <gecode/driver.hh> 
#include <gecode/int.hh>              
#include <gecode/minimodel.hh>     
#include <gecode/Heuristic/LReSBDSBrancher_incNGs_MSA+MFDF.hpp>   
using namespace Gecode;     

int s_n,s_nn;    
      
int soln=0; 
int sol;  
IntArgs PerA; 
Index_class _sym (int id, int index, int val){    
  
	if(id==1)
	{
		int a=index%s_n;
		int b=s_n-1-index/s_n;
		return Index_class(a*s_n+b,val);
	}

	if(id==5)
	{
		int a=s_n-1-index/s_n;
		int b=s_n-1-index%s_n;
		return Index_class(a*s_n+b,val); 
	} 

	if(id==2)
	{	int a=s_n-1-index%s_n;  
		int b=index/s_n;
		return Index_class(a*s_n+b,val);} 

	if(id==0)
	{  	int a=s_n-1-index/s_n; 
		int b=index%s_n;
		return Index_class(a*s_n+b,val);} 
  
	if(id==4)   
	{	int a=index/s_n;
		int b=s_n-1-index%s_n;
		return Index_class(a*s_n+b,val); } 
  
	if(id==3) 
	{ 	int a=index%s_n;
		int b=index/s_n;
		return Index_class(a*s_n+b,val); }

	if(id==6)
	{ 	int a=s_n-1-index%s_n;
		int b=s_n-1-index/s_n;
		return Index_class(a*s_n+b,val);}
  
	if(id>6)
		if(val==id-6) return Index_class(index,val+1);
		else if(val==id-5) return Index_class(index,val-1); 
	return Index_class(index,val);
 
}    
    
//**************************************************// 
//**************************************************//
//**************************************************//
//**************************************************//
//**************************************************//
//**************************************************//

   

 
int pn(int a)
{
	if(a==1) return 1; 
	else return a*pn(a-1);
}

  
class Perm : public Space { 
	protected: 
	   IntVarArray q; 
	public: 
	   Perm(void) :q(*this,s_n,1,s_n) { 
		  distinct(*this,q);
		  branch(*this, q, INT_VAR_NONE(), INT_VAL_MIN()); 
	   } 
	   // Search support 
	   Perm(bool share, Perm& s) : Space(share, s) {
		  q.update(*this, share, s.q); 
	   }   
	   virtual Space* copy(bool share) {
		  return new Perm(share,*this); 
	   } 
	   // Print solution 
	   void print(void) const {
		  for(int i=0;i<q.size();i++)
			PerA[sol*s_n+i]=q[i].val();
		  sol++;
	   } 
	   
}; 

class DLS : public Script { 
private:
  /// Size of magic square
  const int n;
  /// Fields of square  
  IntVarArray x;

public:
  /// Post constraints
  DLS(const SizeOptions& opt)
    : n(opt.size()), x(*this,n*n,1,n) { 
    // Number of fields on square
 
	PerA=IntArgs(pn(opt.size())*opt.size());
    const int nn = n*n;
    s_n=n;
    s_nn=nn;
    // Sum of all a row, column, or diagonal
    const int s  = nn*(nn+1) / (2*n); 

    // Matrix-wrapper for the square
    Matrix<IntVarArray> m(x, n, n);

    for (int i = n; i--; ) { 
      distinct(*this, m.row(i));
      distinct(*this, m.col(i));
    } 
    // Both diagonals must have sum s
    {
      IntVarArgs d1y(n);
      IntVarArgs d2y(n);
      for (int i = n; i--; ) {
        d1y[i] = m(i,i);
        d2y[i] = m(n-i-1,i);
      } 
      distinct(*this, d1y);
      distinct(*this, d2y);  
    }

 
    Perm* ms = new Perm;    
	DFS<Perm> e(ms);        
	delete ms; 
	// Use the search engine to find all solutions  
	while (Perm*sf =e.next()) { 
		sf->print(); delete sf;
	}
	std::cout<<"the sol is "<<sol<<"\n";
	  
	branch(*this, x, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),7+n-1,_sym,n+1);
  }        
  
  /// Constructor for cloning \a s
  DLS(bool share, DLS& s) : Script(share,s), n(s.n) {   
    x.update(*this, share, s.x);
  }
 
  /// Copy during cloning
  virtual Space* 
  copy(bool share) {
    return new DLS(share,*this);  
  }
  /// Print solution
  virtual void
  print(std::ostream& os) const { 
 } 

};

/** \brief Main-function
 *  \relates DLS
 */
int
main(int argc, char* argv[]) {
  SizeOptions opt("DLS");
  opt.iterations(1);
  opt.size(3);
  opt.solutions(0);
  opt.c_d(1);
  opt.parse(argc,argv);
  s_n=opt.size();
  Script::run<DLS,DFS,SizeOptions>(opt);
  std::cout<<soln<<"\n";
  return 0;
}

 

