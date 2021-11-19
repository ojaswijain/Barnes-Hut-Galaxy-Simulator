#include "particle.cpp"
#include "tree.cpp"

/**
 * @brief Non GUI version to test for errors in the tree structure
 * Prints the coordinates of a certain particle to track progress 
 */

using std::cout;

signed main(){

    vector <particle*> ps;
    
    /**
     * Creating a random set of particles 
     * Inserting them in the particle set ps defined above
     */

    for(int i=0;i<1000;i++){
        particle* p = new particle (201+rand()%600, 201+rand()%600, 1e-3*(1+rand()%1000));
        ps.push_back(p);
    }
    
    for(;;){

        /**
         * blank refers to the central particle
         * parameters of blank can be changed to observe different evolution patterns
         * Creating a new tree at the beginning of each iteration (time unit)
         * inserting the particleset in the tree
         */

    	particle* blank = new particle (0,0,0);
        tree* Tree = new tree(0,1000,0,1000,blank);
        Tree->add(ps);

        /**
         *Using updateAcc to iteratively update the force on each particle 
         * Using equations of motion for newLoc and newVel 
         */

        for (auto x : ps){
            Tree->updateAcc(x);
        }
        
        for (auto x : ps){
            x->newLoc();
            x->newVel();
        }

        /**
         * Extracting, and printing coordinates of a certain particle  
         * Resetting all forces, only to evaluate again at next iteration
         */

         double x = ps[0]->X();
         double y = ps[0]->Y();
         cout<<x<<" "<<y<<"\n";
         for (auto x : ps){
            x->resetForce();
        }

        /**
         * Deleting the pointer to the tree  
         * Recursively deleting all child pointers as well.
         */
        
        Tree->clear();
        delete Tree;
     }

    return 0;
}
