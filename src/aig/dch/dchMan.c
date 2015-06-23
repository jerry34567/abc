/**CFile****************************************************************

  FileName    [dchMan.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Choice computation for tech-mapping.]

  Synopsis    [Calls to the SAT solver.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 29, 2008.]

  Revision    [$Id: dchMan.c,v 1.00 2008/07/29 00:00:00 alanmi Exp $]

***********************************************************************/

#include "dchInt.h"

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Creates the manager.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
Dch_Man_t * Dch_ManCreate( Aig_Man_t * pAig, Dch_Pars_t * pPars )
{
    Dch_Man_t * p;
    // create interpolation manager
    p = ABC_ALLOC( Dch_Man_t, 1 );
    memset( p, 0, sizeof(Dch_Man_t) );
    p->pPars        = pPars;
    p->pAigTotal    = pAig; //Dch_DeriveTotalAig( vAigs );
    Aig_ManFanoutStart( p->pAigTotal );
    // SAT solving
    p->nSatVars     = 1;
    p->pSatVars     = ABC_CALLOC( int, Aig_ManObjNumMax(p->pAigTotal) );
    p->vUsedNodes   = Vec_PtrAlloc( 1000 );
    p->vFanins      = Vec_PtrAlloc( 100 );
    p->vSimRoots    = Vec_PtrAlloc( 1000 );
    p->vSimClasses  = Vec_PtrAlloc( 1000 );
    // equivalences proved
    p->pReprsProved = ABC_CALLOC( Aig_Obj_t *, Aig_ManObjNumMax(p->pAigTotal) );
    return p;
}

/**Function*************************************************************

  Synopsis    [Prints stats of the manager.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Dch_ManPrintStats( Dch_Man_t * p )
{
    int nNodeNum = Aig_ManNodeNum(p->pAigTotal) / 3;
    printf( "Parameters: Sim words = %d. Conf limit = %d. SAT var max = %d.\n", 
        p->pPars->nWords, p->pPars->nBTLimit, p->pPars->nSatVarMax );
    printf( "AIG nodes : Total = %6d. Dangling = %6d. Main = %6d. (%6.2f %%)\n", 
        Aig_ManNodeNum(p->pAigTotal), 
        Aig_ManNodeNum(p->pAigTotal)-nNodeNum,
        nNodeNum,
        100.0 * nNodeNum/Aig_ManNodeNum(p->pAigTotal) );
    printf( "SAT solver: Vars = %d. Max cone = %d. Recycles = %d.\n", 
        p->nSatVars, p->nConeMax, p->nRecycles );
    printf( "SAT calls : All = %6d. Unsat = %6d. Sat = %6d. Fail = %6d.\n", 
        p->nSatCalls, p->nSatCalls-p->nSatCallsSat-p->nSatFailsReal, 
        p->nSatCallsSat, p->nSatFailsReal );
    printf( "Choices   : Lits = %6d. Reprs = %5d. Equivs = %5d. Choices = %5d.\n", 
        p->nLits, p->nReprs, p->nEquivs, p->nChoices );
    printf( "Choicing runtime statistics:\n" );
    p->timeOther = p->timeTotal-p->timeSimInit-p->timeSimSat-p->timeSat-p->timeChoice;
    ABC_PRTP( "Sim init   ", p->timeSimInit,  p->timeTotal );
    ABC_PRTP( "Sim SAT    ", p->timeSimSat,   p->timeTotal );
    ABC_PRTP( "SAT solving", p->timeSat,      p->timeTotal );
    ABC_PRTP( "  sat      ", p->timeSatSat,   p->timeTotal );
    ABC_PRTP( "  unsat    ", p->timeSatUnsat, p->timeTotal );
    ABC_PRTP( "  undecided", p->timeSatUndec, p->timeTotal );
    ABC_PRTP( "Choice     ", p->timeChoice,   p->timeTotal );
    ABC_PRTP( "Other      ", p->timeOther,    p->timeTotal );
    ABC_PRTP( "TOTAL      ", p->timeTotal,    p->timeTotal );
    if ( p->pPars->timeSynth )
    {
    ABC_PRT( "Synthesis  ", p->pPars->timeSynth );
    }
} 

/**Function*************************************************************

  Synopsis    [Frees the manager.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Dch_ManStop( Dch_Man_t * p )
{
    Aig_ManFanoutStop( p->pAigTotal );
    if ( p->pPars->fVerbose )
        Dch_ManPrintStats( p );
    if ( p->pAigFraig )
        Aig_ManStop( p->pAigFraig );
    if ( p->ppClasses )
        Dch_ClassesStop( p->ppClasses );
    if ( p->pSat )
        sat_solver_delete( p->pSat );
    Vec_PtrFree( p->vUsedNodes );
    Vec_PtrFree( p->vFanins );
    Vec_PtrFree( p->vSimRoots );
    Vec_PtrFree( p->vSimClasses );
    ABC_FREE( p->pReprsProved );
    ABC_FREE( p->pSatVars );
    ABC_FREE( p );
}

/**Function*************************************************************

  Synopsis    [Recycles the SAT solver.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Dch_ManSatSolverRecycle( Dch_Man_t * p )
{
    int Lit;
    if ( p->pSat )
    {
        Aig_Obj_t * pObj;
        int i;
        Vec_PtrForEachEntry( p->vUsedNodes, pObj, i )
            Dch_ObjSetSatNum( p, pObj, 0 );
        Vec_PtrClear( p->vUsedNodes );
//        memset( p->pSatVars, 0, sizeof(int) * Aig_ManObjNumMax(p->pAigTotal) );
        sat_solver_delete( p->pSat );
    }
    p->pSat = sat_solver_new();
    sat_solver_setnvars( p->pSat, 1000 );
    // var 0 is not used
    // var 1 is reserved for const1 node - add the clause
    p->nSatVars = 1;
//    p->nSatVars = 0;
    Lit = toLit( p->nSatVars );
    if ( p->pPars->fPolarFlip )
        Lit = lit_neg( Lit );
    sat_solver_addclause( p->pSat, &Lit, &Lit + 1 );
    Dch_ObjSetSatNum( p, Aig_ManConst1(p->pAigFraig), p->nSatVars++ );

    p->nRecycles++;
    p->nCallsSince = 0;
}




////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


