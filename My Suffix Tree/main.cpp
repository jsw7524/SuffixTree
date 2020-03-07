#include <iostream>
#include <cstring>
using namespace std;

/*========Data========*/
class Edge;
class ExplicitState;

int A;

class ExplicitState
{
public:
    Edge* EdgeIndex[256];
    ExplicitState * SuffixLink;

    ExplicitState()
    {

        for (A = 0 ; A < 256 ; A++)
        {
            EdgeIndex[A]=NULL;

        }

    }

}MyRoot,MyLeaf,AuxiliaryState;

class Edge
{
public:
    static int Len;
    int S;
    int E;
    ExplicitState * ToState;
    bool IsLeaf;
};
int Edge::Len=0;

class ReferencePair
{
public:
    ExplicitState * ClosestState ;
    int K;
};

class TestPair
{
public:
    bool IsEndPoint;
    ExplicitState * ClosestState ;
};

char InputChar;
char MyString[10000000];
int  StringLen=0;

/*========Algorithm========*/

TestPair MyTestAndSplit(ExplicitState * S,int K,int P,char T)
{
    TestPair Temp;
    int K1,P1;
    ExplicitState * TS;
    if ( K <= P )
    {
        K1=S->EdgeIndex[ MyString[K] ]->S;

        if (S->EdgeIndex[ MyString[K] ]->IsLeaf == false)
            P1=S->EdgeIndex[ MyString[K] ]->E;
        else
            P1=S->EdgeIndex[MyString[K]]->Len;

        TS=S->EdgeIndex[MyString[K]]->ToState;

        if ( MyString[K1+P-K+1] == T )
        {
            Temp.IsEndPoint=true;
            Temp.ClosestState=S;
            return Temp;
        }
        else
        {
            //printf("New State\n");
            Temp.ClosestState =new ExplicitState;
            /////////////
            Temp.ClosestState->EdgeIndex[ MyString[K1+P-K+1] ] = new Edge;
            Temp.ClosestState->EdgeIndex[ MyString[K1+P-K+1] ]->S=K1+P-K+1;
            Temp.ClosestState->EdgeIndex[ MyString[K1+P-K+1] ]->E=P1;
            Temp.ClosestState->EdgeIndex[ MyString[K1+P-K+1] ]->ToState=TS;
            Temp.ClosestState->EdgeIndex[ MyString[K1+P-K+1] ]->IsLeaf=S->EdgeIndex[MyString[K]]->IsLeaf;
            //printf("New Edge %d %d \n\n",K1+P-K+1,P1);
            /////////////////////
            S->EdgeIndex[MyString[K]]->E=K1+P-K;
            S->EdgeIndex[MyString[K]]->ToState=Temp.ClosestState;
            S->EdgeIndex[MyString[K]]->IsLeaf=false;

            Temp.IsEndPoint=false;
            return Temp;

        }

    }
    else
    {
        if ( S->EdgeIndex[T]== NULL )  //?
        {
            Temp.IsEndPoint=false;
            Temp.ClosestState=S;
            return Temp;
        }
        else
        {
            Temp.IsEndPoint=true;
            Temp.ClosestState=S;
            return Temp;
        }
    }
}

ReferencePair MyCanonize(ExplicitState * S,int K,int P)
{
    ReferencePair MyReferencePair;
    int K1,P1;
    if ( P < K )
    {
        MyReferencePair.ClosestState = S;
        MyReferencePair.K=K;
        return MyReferencePair;
    }
    else
    {
        K1=S->EdgeIndex[MyString[K]]->S;
        if (S->EdgeIndex[MyString[K]]->IsLeaf)
            P1=S->EdgeIndex[MyString[K]]->Len;
        else
            P1=S->EdgeIndex[MyString[K]]->E;

        while ( P1-K1 <= P-K )
        {
            K=K+P1-K1+1;


            if (S->EdgeIndex[MyString[K1]]->ToState != &MyLeaf)
            {
                S=S->EdgeIndex[MyString[K1]]->ToState;              ////////////????????????
            }

            if ( K <= P )
            {
                K1=S->EdgeIndex[MyString[K]]->S;
                //////////////////////////////////
                if (S->EdgeIndex[ MyString[K] ]->IsLeaf == false)
                    P1=S->EdgeIndex[ MyString[K] ]->E;
                else
                    P1=S->EdgeIndex[MyString[K]]->Len;
                //////////////////////////////////

            }
        }

        MyReferencePair.ClosestState=S;
        MyReferencePair.K=K;
        return MyReferencePair;
    }
}

ReferencePair MyUpdate(ExplicitState * S,int K,int I)
{
    ExplicitState * Older=&MyRoot;
    TestPair MyTemp=MyTestAndSplit(S,K,I-1,MyString[I]);
    ReferencePair MyReferencePair={MyTemp.ClosestState,K};

    while (!MyTemp.IsEndPoint)
    {
        MyTemp.ClosestState->EdgeIndex[ MyString[I] ] = new Edge;
        MyTemp.ClosestState->EdgeIndex[ MyString[I] ]->S=I;
        MyTemp.ClosestState->EdgeIndex[ MyString[I] ]->E=StringLen;

        MyTemp.ClosestState->EdgeIndex[ MyString[I] ]->IsLeaf=true;
        MyTemp.ClosestState->EdgeIndex[ MyString[I] ]->ToState=&MyLeaf;

        //printf("New Edge %d %d \n\n",I,StringLen);

        if (Older != &MyRoot )
        {
            Older->SuffixLink=MyTemp.ClosestState;
        }
        Older = MyTemp.ClosestState;

        MyReferencePair = MyCanonize(S->SuffixLink,K,I-1);
        S=MyReferencePair.ClosestState;
        K=MyReferencePair.K;

        MyTemp=MyTestAndSplit(S,K,I-1,MyString[I]);
    }

    if (Older != &MyRoot )
    {
        Older->SuffixLink=MyTemp.ClosestState;
    }

    return MyReferencePair;
}

void DisplayTree(ExplicitState * MyPtr,int MyOffSet)
{
    int TS,TE,TK;
    int TT;
    for (TT=0 ; TT < 256 ; TT++)
    {

        if ( MyPtr->EdgeIndex[ TT ] != NULL )
        {
            //printf("X%cX\n",TT);
            TS=MyPtr->EdgeIndex[ TT ]->S;
            if ( MyPtr->EdgeIndex[ TT ]->IsLeaf == true)
            {
                TE=Edge::Len;
            }
            else
            {
                TE=MyPtr->EdgeIndex[ TT ]->E;
            }

            for (TK=0;TK<MyOffSet;TK++)
            {
                printf(" ");
            }

            for (TK=TS;TK<=TE;TK++)
            {
                printf("%c",MyString[TK]);
            }
            printf("\n");
            DisplayTree( MyPtr->EdgeIndex[ TT ]->ToState,MyOffSet+(TE-TS)+2 );
            //printf("\n");
        }
    }
    return;
}

void FindSubString()
{

    ExplicitState * Ptr;
    char SubString[1000];
    int PtrLen=1;
    int SunLen=0;
    int MyStart,MyEnd,J;

    while (EOF != scanf("%s",SubString+1))
    {
        SunLen=strlen(SubString+1);
        Ptr = &MyRoot;
        PtrLen=1;

        if ( Ptr->EdgeIndex[ SubString[PtrLen] ] == NULL)
        {
            printf("No\n");
            goto S1;
        }

        while (1)
        {
            if ( MyStart=Ptr->EdgeIndex[ SubString[PtrLen] ] == NULL )
            {
                printf("No\n");
                goto S1;
            }
            else
            {
                MyStart=Ptr->EdgeIndex[ SubString[PtrLen] ]->S;

                if (Ptr->EdgeIndex[ SubString[PtrLen] ]->IsLeaf==true )
                {
                    MyEnd=Edge::Len;
                }
                else
                {
                    MyEnd=Ptr->EdgeIndex[ SubString[PtrLen] ]->E;     ////infinite Leaf ?
                }
            }

            for (J = MyStart ; J <=  MyEnd && PtrLen <= SunLen  ; J++)
            {
                if ( SubString[PtrLen]== MyString[J] )
                {
                    PtrLen++;
                }
                else
                {
                    printf("No\n");
                    goto S1;
                }
            }

            if ( PtrLen > SunLen )
            {
                printf("Yes\n");
                goto S1;
            }

            if ( Ptr->EdgeIndex[ MyString[MyStart] ] != NULL)
            {
                Ptr=Ptr->EdgeIndex[ MyString[MyStart] ]->ToState;
            }
            else
            {
                printf("No\n");
                goto S1;
            }

        }
S1:
        ;

    }
}


int main()
{

    ReferencePair MyPair={&MyRoot,1};

    for (A = 0 ; A < 256 ; A++)
    {
        AuxiliaryState.EdgeIndex[A]=new Edge;
        AuxiliaryState.EdgeIndex[A]->S=0;
        AuxiliaryState.EdgeIndex[A]->E=0;
        AuxiliaryState.EdgeIndex[ A ]->IsLeaf=false;
        AuxiliaryState.EdgeIndex[A]->ToState=&MyRoot;
    }
    AuxiliaryState.SuffixLink=&MyRoot;
    MyRoot.SuffixLink=&AuxiliaryState;  //

    while (1)
    {
        InputChar=getchar();
        if ( InputChar == '\n' )
        {
            break;
        }

        StringLen++;
        Edge::Len=StringLen;
        MyString[StringLen]=InputChar;

        MyPair=MyUpdate(MyPair.ClosestState,MyPair.K,StringLen);
        printf("End Point Pair %d %d %d\n",StringLen,MyPair.K,MyPair.ClosestState);
        MyPair=MyCanonize(MyPair.ClosestState,MyPair.K,StringLen);
        printf("Canonize %d %d %d\n",StringLen,MyPair.K,MyPair.ClosestState);
        printf("---------\n");

    }

    printf("Suffix Tree\n");
    DisplayTree(&MyRoot,0);
    printf("FindSubString\n");
    FindSubString();
    return 0;

}
