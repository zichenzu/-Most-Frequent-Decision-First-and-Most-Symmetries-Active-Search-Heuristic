
namespace Gecode {
 
	//index_class
	class Index_class{
		public: 
			int index;
			int val;
			~Index_class(){};
			Index_class(int _index,int _val){index=_index;val=_val;};
				
	  };
	  
	typedef Index_class (*SymmetryFunction)(int ,int,int);
	int sr;
	SymmetryFunction _symmetries;
	//symmetry class
	class SBDS_Sym  {
        private:
        Space& _home;
            
        	

        public:
            int _nSym;
			IntArgs _nSymR;
			int mystack;
		 
			
			~SBDS_Sym() {} ;
			Space& getManager() {return _home;};
          
			int getNSym() {return _nSym;};
 

			void symGoal(ViewArray<Int::IntView>& vars, int index, int val );//-----------------------------this is the normal SBDS adds conditional contrains

			SBDS_Sym(Space& home, int nSym,int var_n,int dosize) ;
			void SymAdjust(ViewArray<Int::IntView>& vars, int index, int val) ;
			SBDS_Sym(Space& home, SBDS_Sym OldSym) ;
	};



}


  

