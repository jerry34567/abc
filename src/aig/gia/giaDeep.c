/**CFile****************************************************************

  FileName    [giaDeep.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Scalable AIG package.]

  Synopsis    [Experiments with synthesis.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: giaDeep.c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#include "gia.h"
#include "base/main/main.h"
#include "base/cmd/cmd.h"

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
// Gia_Man_t * Gia_ManDeepSynOne( int nNoImpr, int TimeOut, int nAnds, int Seed, int fUseTwo, int fVerbose )
// {
//     abctime nTimeToStop = TimeOut ? Abc_Clock() + TimeOut * CLOCKS_PER_SEC : 0;
//     abctime clkStart    = Abc_Clock();
//     int s, i, IterMax   = 100000, nAndsMin = -1, iIterLast = -1;
//     Gia_Man_t * pTemp   = Abc_FrameReadGia(Abc_FrameGetGlobalFrame());
//     Gia_Man_t * pNew    = Gia_ManDup( pTemp );
//     Abc_Random(1);

//     // TEST
//     int if_jf = 1; // &if: true, &jf: false
//     // ENDTEST
//     for ( s = 0; s < 10+Seed; s++ )
//         Abc_Random(0);
//     for ( i = 0; i < IterMax; i++ )
//     {
//         unsigned Rand = Abc_Random(0);
//         int fDch = Rand & 1;
//         //int fCom = (Rand >> 1) & 3;
//         int fCom = (Rand >> 1) & 1;
//         int fFx  = (Rand >> 2) & 1;
//         int KLut = fUseTwo ? 2 + (i % 5) : 3 + (i % 4);
//         if_jf = (Rand >> 3) & 10 ? if_jf : 1 - if_jf; // only 1/8 will toggle, other 7/8 will remain the same
//         int fChange = 0;
//         char Command[1000];
//         char * pComp = NULL;
//         if ( fCom == 3 )
//             pComp = "; &put; compress2rs; compress2rs; compress2rs; &get";
//         else if ( fCom == 2 )
//             pComp = "; &put; compress2rs; compress2rs; &get";
//         else if ( fCom == 1 )
//             pComp = "; &put; compress2rs; &get";
//         else if ( fCom == 0 )
//             pComp = "; &dc2";
//         sprintf( Command, "&dch%s; %s -a -K %d; &mfs -e -W 20 -L 20%s%s",
//             fDch ? " -f" : "", if_jf ? "&if" : "&jf", KLut, fFx ? "; &fx; &st" : "", pComp );
//         // printf("ALL: %s\n", Command);
//         if ( Abc_FrameIsBatchMode() )
//         {
//             if ( Cmd_CommandExecute(Abc_FrameGetGlobalFrame(), Command) )
//             {
//                 Abc_Print( 1, "Something did not work out with the command \"%s\".\n", Command );
//                 return NULL;
//             }
//         }
//         else
//         {
//             Abc_FrameSetBatchMode( 1 );
//             if ( Cmd_CommandExecute(Abc_FrameGetGlobalFrame(), Command) )
//             {
//                 Abc_Print( 1, "Something did not work out with the command \"%s\".\n", Command );
//                 return NULL;
//             }
//             Abc_FrameSetBatchMode( 0 );
//         }
//         pTemp = Abc_FrameReadGia(Abc_FrameGetGlobalFrame());
//         if ( Gia_ManAndNum(pNew) > Gia_ManAndNum(pTemp) )
//         // if ( Gia_ManAndNum(pNew) + Gia_ManLevelNum(pNew) > Gia_ManAndNum(pTemp) + Gia_ManLevelNum(pTemp))  //// test
//         {
//             Gia_ManStop( pNew );
//             pNew = Gia_ManDup( pTemp );
//             fChange = 1;
//             iIterLast = i;
//         }
//         // else if ( Gia_ManAndNum(pNew) + Gia_ManAndNum(pNew)/10 < Gia_ManAndNum(pTemp) ) 
//         // else if ( Gia_ManAndNum(pNew) + Gia_ManAndNum(pNew)/5 + Gia_ManLevelNum(pNew) < Gia_ManAndNum(pTemp) + Gia_ManLevelNum(pTemp) ) 
//         else
//         {
//             //printf( "Updating\n" );
//             //Abc_FrameUpdateGia( Abc_FrameGetGlobalFrame(), Gia_ManDup(pNew) );
//         }
//         // TEST
//         if ( TimeOut && fChange )
//         {
//             if (nTimeToStop < Abc_Clock() + 150 * CLOCKS_PER_SEC && nTimeToStop < clkStart + 3000 * CLOCKS_PER_SEC) 
//             {
//                 nTimeToStop += 150 * CLOCKS_PER_SEC;
//             }
//         }
//         // ENDTEST
//         if ( fChange && fVerbose )
//         {
//             printf( "Iter %6d : ", i );
//             printf( "Time %8.2f sec : ", (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
//             printf( "And = %6d  ", Gia_ManAndNum(pNew) );
//             printf( "Lev = %3d  ", Gia_ManLevelNum(pNew) );
//             if ( fChange ) 
//                 printf( "<== best : " );
//             else if ( fVerbose )
//                 printf( "           " );
//             printf( "%s", Command );
//             printf( "\n" );
//         }
//         if ( nTimeToStop && Abc_Clock() > nTimeToStop )
//         {
//             if ( !Abc_FrameIsBatchMode() )
//             printf( "Runtime limit (%d sec) is reached after %d iterations.\n", TimeOut, i );
//             break;
//         }
//         if ( i - iIterLast > nNoImpr )
//         {
//             printf( "Completed %d iterations without improvement in %.2f seconds.\n", 
//                 nNoImpr, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
//             break;
//         }
//     }
//     if ( i == IterMax )
//         printf( "Iteration limit (%d iters) is reached after %.2f seconds.\n", IterMax, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
//     else if ( nAnds && nAndsMin <= nAnds )
//         printf( "Quality goal (%d nodes <= %d nodes) is achieved after %d iterations and %.2f seconds.\n", 
//             nAndsMin, nAnds, i, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
//     return pNew;
// }

//orig
// Gia_Man_t * Gia_ManDeepSynOne( int nNoImpr, int TimeOut, int nAnds, int Seed, int fUseTwo, int fVerbose, int fDisturb )
// {
//     abctime nTimeToStop = TimeOut ? Abc_Clock() + TimeOut * CLOCKS_PER_SEC : 0;
//     abctime clkStart    = Abc_Clock();
//     int s, i, IterMax   = 100000, nAndsMin = -1, iIterLast = -1;
//     int disturb_iter = 0;
//     Gia_Man_t * pTemp   = Abc_FrameReadGia(Abc_FrameGetGlobalFrame());
//     Gia_Man_t * pNew    = Gia_ManDup( pTemp );
//     Abc_Random(1);

//     // TEST
//     int if_jf = 1; // &if: true, &jf: false
//     // ENDTEST
//     for ( s = 0; s < 10+Seed; s++ )
//         Abc_Random(0);
//     for ( i = 0; i < IterMax; i++ )
//     {
//         unsigned Rand = Abc_Random(0);
//         int fDch = Rand & 1;
//         //int fCom = (Rand >> 1) & 3;
//         int fCom = (Rand >> 1) & 1;
//         int fFx  = (Rand >> 2) & 1;
//         int KLut = fUseTwo ? 2 + (i % 5) : 3 + (i % 4);
//         if_jf = (Rand >> 3) & 7 ? if_jf : 1 - if_jf; // only 1/8 will toggle, other 7/8 will remain the same
//         int fChange = 0;
//         char Command[1000];
//         char * pComp = NULL;
//         if ( fCom == 3 )
//             pComp = "; &put; compress2rs; compress2rs; compress2rs; &get";
//         else if ( fCom == 2 )
//             pComp = "; &put; compress2rs; compress2rs; &get";
//         else if ( fCom == 1 )
//             pComp = "; &put; compress2rs; &get";
//         else if ( fCom == 0 )
//             pComp = "; &dc2";
//         sprintf( Command, "&dch%s; %s -a -K %d; &mfs -e -W 20 -L 20%s%s",
//             fDch ? " -f" : "", if_jf ? "&if" : "&jf", KLut, fFx ? "; &fx; &st" : "", pComp );
//         // printf("ALL: %s\n", Command);
//         if ( Abc_FrameIsBatchMode() )
//         {
//             if ( Cmd_CommandExecute(Abc_FrameGetGlobalFrame(), Command) )
//             {
//                 Abc_Print( 1, "Something did not work out with the command \"%s\".\n", Command );
//                 return NULL;
//             }
//         }
//         else
//         {
//             Abc_FrameSetBatchMode( 1 );
//             if ( Cmd_CommandExecute(Abc_FrameGetGlobalFrame(), Command) )
//             {
//                 Abc_Print( 1, "Something did not work out with the command \"%s\".\n", Command );
//                 return NULL;
//             }
//             Abc_FrameSetBatchMode( 0 );
//         }
//         pTemp = Abc_FrameReadGia(Abc_FrameGetGlobalFrame());
//         // if ( Gia_ManAndNum(pNew) > Gia_ManAndNum(pTemp) )
//         if ( Gia_ManAndNum(pNew) + Gia_ManLevelNum(pNew) > Gia_ManAndNum(pTemp) + Gia_ManLevelNum(pTemp))  //// test
//         {
//             Gia_ManStop( pNew );
//             pNew = Gia_ManDup( pTemp );
//             fChange = 1;
//             iIterLast = i;
//         }
//         // else if ( Gia_ManAndNum(pNew) + Gia_ManAndNum(pNew)/10 < Gia_ManAndNum(pTemp) ) 
//         // else if ( Gia_ManAndNum(pNew) + Gia_ManAndNum(pNew)/5 + Gia_ManLevelNum(pNew) < Gia_ManAndNum(pTemp) + Gia_ManLevelNum(pTemp) ) 
//         else
//         {
//             if (fDisturb) {
//                 Gia_ManStop( pNew );
//                 pNew = Gia_ManDup( pTemp );
//                 disturb_iter++;
//                 // printf("disturb!!!!\n");
//             }
//             //printf( "Updating\n" );
//             //Abc_FrameUpdateGia( Abc_FrameGetGlobalFrame(), Gia_ManDup(pNew) );
//         }
//         // TEST
//         if ( TimeOut && fChange )
//         {
//             if (nTimeToStop < Abc_Clock() + 300 * CLOCKS_PER_SEC && nTimeToStop < clkStart + 3000 * CLOCKS_PER_SEC) 
//             {
//                 nTimeToStop += 300 * CLOCKS_PER_SEC;
//             }
//         }
//         // ENDTEST
//         if ( fChange && fVerbose )
//         {
//             printf( "Iter %6d : ", i );
//             printf( "Time %8.2f sec : ", (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
//             printf( "And = %6d  ", Gia_ManAndNum(pNew) );
//             printf( "Lev = %3d  ", Gia_ManLevelNum(pNew) );
//             if ( fChange ) 
//                 printf( "<== best : " );
//             else if ( fVerbose )
//                 printf( "           " );
//             printf( "%s", Command );
//             printf( "\n" );
//         }
//         if ( nTimeToStop && Abc_Clock() > nTimeToStop )
//         {
//             if ( !Abc_FrameIsBatchMode() )
//             printf( "Runtime limit (%d sec) is reached after %d iterations.\n", TimeOut, i );
//             break;
//         }
//         if ( i - iIterLast > nNoImpr )
//         {
//             printf( "Completed %d iterations without improvement in %.2f seconds.\n", 
//                 nNoImpr, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
//             break;
//         }
//         if (fDisturb && disturb_iter == nNoImpr) {
//             printf("disterb %d round complete.\n", nNoImpr);
//             break;
//         }
//     }
//     if ( i == IterMax )
//         printf( "Iteration limit (%d iters) is reached after %.2f seconds.\n", IterMax, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
//     else if ( nAnds && nAndsMin <= nAnds )
//         printf( "Quality goal (%d nodes <= %d nodes) is achieved after %d iterations and %.2f seconds.\n", 
//             nAndsMin, nAnds, i, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
//     return pNew;
// }
Gia_Man_t * Gia_ManDeepSynOne( int nNoImpr, int TimeOut, int nAnds, int Seed, int fUseTwo, int fVerbose, int fDisturb )
{
    abctime nTimeToStop = TimeOut ? Abc_Clock() + TimeOut * CLOCKS_PER_SEC : 0;
    abctime clkStart    = Abc_Clock();
    int s, i, IterMax   = 100000, nAndsMin = -1, iIterLast = -1;
    int disturb_iter = 0;
    Gia_Man_t * pTemp   = Abc_FrameReadGia(Abc_FrameGetGlobalFrame());
    Gia_Man_t * pNew    = Gia_ManDup( pTemp );
    Abc_Random(1);

    // TEST
    int if_jf = 1; // &if: true, &jf: false
    // ENDTEST
    for ( s = 0; s < 10+Seed; s++ )
        Abc_Random(0);
    for ( i = 0; i < IterMax; i++ )
    {
        unsigned Rand = Abc_Random(0);
        int fDch = Rand & 1;
        //int fCom = (Rand >> 1) & 3;
        int fCom = (Rand >> 1) & 1;
        int fFx  = (Rand >> 2) & 1;
        // int KLut = fUseTwo ? 2 + (i % 5) : 3 + (i % 4);
        int KLut = fUseTwo ? 2 + (i % 7) : 3 + (i % 6);
        if_jf = (Rand >> 3) & 10 ? if_jf : 1 - if_jf; // only 1/8 will toggle, other 7/8 will remain the same
        int fChange = 0;
        char Command[1000];
        char * pComp = NULL;
        if ( fCom == 3 )
            pComp = "; &put; compress2rs; compress2rs; compress2rs; &get";
        else if ( fCom == 2 )
            pComp = "; &put; compress2rs; compress2rs; &get";
        else if ( fCom == 1 )
            pComp = "; &put; compress2rs; &get";
        else if ( fCom == 0 )
            pComp = "; &dc2";
        sprintf( Command, "&dch%s; %s -a -K %d; &mfs -e -W 20 -L 20%s%s",
            fDch ? " -f" : "", if_jf ? "&if" : "&jf", KLut, fFx ? "; &fx; &st" : "", pComp );
        // printf("ALL: %s\n", Command);
        if ( Abc_FrameIsBatchMode() )
        {
            if ( Cmd_CommandExecute(Abc_FrameGetGlobalFrame(), Command) )
            {
                Abc_Print( 1, "Something did not work out with the command \"%s\".\n", Command );
                return NULL;
            }
        }
        else
        {
            Abc_FrameSetBatchMode( 1 );
            if ( Cmd_CommandExecute(Abc_FrameGetGlobalFrame(), Command) )
            {
                Abc_Print( 1, "Something did not work out with the command \"%s\".\n", Command );
                return NULL;
            }
            Abc_FrameSetBatchMode( 0 );
        }
        pTemp = Abc_FrameReadGia(Abc_FrameGetGlobalFrame());
        if ( Gia_ManAndNum(pNew) > Gia_ManAndNum(pTemp) )
        // if ( Gia_ManAndNum(pNew) + Gia_ManLevelNum(pNew) > Gia_ManAndNum(pTemp) + Gia_ManLevelNum(pTemp))  //// test
        {
            Gia_ManStop( pNew );
            pNew = Gia_ManDup( pTemp );
            fChange = 1;
            iIterLast = i;
        }
        // else if ( Gia_ManAndNum(pNew) + Gia_ManAndNum(pNew)/10 < Gia_ManAndNum(pTemp) ) 
        // else if ( Gia_ManAndNum(pNew) + Gia_ManAndNum(pNew)/5 + Gia_ManLevelNum(pNew) < Gia_ManAndNum(pTemp) + Gia_ManLevelNum(pTemp) ) 
        else
        {
            if (fDisturb) {
                Gia_ManStop( pNew );
                pNew = Gia_ManDup( pTemp );
                disturb_iter++;
                // printf("disturb!!!!\n");
            }
            //printf( "Updating\n" );
            //Abc_FrameUpdateGia( Abc_FrameGetGlobalFrame(), Gia_ManDup(pNew) );
        }
        // TEST
        if ( TimeOut && fChange )
        {
            if (nTimeToStop < Abc_Clock() + 300 * CLOCKS_PER_SEC && nTimeToStop < clkStart + 3000 * CLOCKS_PER_SEC) 
            {
                nTimeToStop += 300 * CLOCKS_PER_SEC;
            }
        }
        // ENDTEST
        if ( fChange && fVerbose )
        {
            printf( "Iter %6d : ", i );
            printf( "Time %8.2f sec : ", (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
            printf( "And = %6d  ", Gia_ManAndNum(pNew) );
            printf( "Lev = %3d  ", Gia_ManLevelNum(pNew) );
            if ( fChange ) 
                printf( "<== best : " );
            else if ( fVerbose )
                printf( "           " );
            printf( "%s", Command );
            printf( "\n" );
        }
        if ( nTimeToStop && Abc_Clock() > nTimeToStop )
        {
            if ( !Abc_FrameIsBatchMode() )
            printf( "Runtime limit (%d sec) is reached after %d iterations.\n", TimeOut, i );
            break;
        }
        if ( i - iIterLast > nNoImpr )
        {
            printf( "Completed %d iterations without improvement in %.2f seconds.\n", 
                nNoImpr, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
            break;
        }
        if (fDisturb && disturb_iter == nNoImpr) {
            printf("disterb %d round complete.\n", nNoImpr);
            break;
        }
    }
    if ( i == IterMax )
        printf( "Iteration limit (%d iters) is reached after %.2f seconds.\n", IterMax, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
    else if ( nAnds && nAndsMin <= nAnds )
        printf( "Quality goal (%d nodes <= %d nodes) is achieved after %d iterations and %.2f seconds.\n", 
            nAndsMin, nAnds, i, (float)1.0*(Abc_Clock() - clkStart)/CLOCKS_PER_SEC );
    return pNew;
}
Gia_Man_t * Gia_ManDeepSyn( Gia_Man_t * pGia, int nIters, int nNoImpr, int TimeOut, int nAnds, int Seed, int fUseTwo, int fVerbose, int fDisturb)
{
    Gia_Man_t * pInit = Gia_ManDup(pGia);
    Gia_Man_t * pBest = Gia_ManDup(pGia);
    Gia_Man_t * pThis;
    int i;
    for ( i = 0; i < nIters; i++ )
    {
        Abc_FrameUpdateGia( Abc_FrameGetGlobalFrame(), Gia_ManDup(pInit) );
        pThis = Gia_ManDeepSynOne( nNoImpr, TimeOut, nAnds, Seed+i, fUseTwo, fVerbose, fDisturb );
        if (fDisturb || (Gia_ManAndNum(pBest) > Gia_ManAndNum(pThis)) ) 
        // if (fDisturb || (Gia_ManAndNum(pBest) > Gia_ManAndNum(pThis)) ) 
        // if (Gia_ManAndNum(pBest) > Gia_ManAndNum(pThis)) 
        {
            Gia_ManStop( pBest );
            pBest = pThis;
        }
        else 
            Gia_ManStop( pThis );
        
    }
    Gia_ManStop( pInit );
    return pBest;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

