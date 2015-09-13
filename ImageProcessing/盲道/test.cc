#include "Matrix.h"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
       Matrix B;
       ifstream ins(argv[1]);
       ins>>B;
       ins.close();
       cout<<B<<endl;
//       cout<<B(1,1)<<endl;
//       cout<<B.det();

//      cout<< B.Minor(3,3)<<endl;
//       cout<<B.Adjugate()<<endl;
//         cout<<B.Cofactor(1,1)<<endl;
       //    cout<<B.Minor(1,2)<<endl;
         //  cout<<B.Cofactor(1,2)<<endl;
        cout<<B*B.Inverse()<<endl;
       
/*
       Matrix C;
       ifstream ins2(argv[2]);
       ins2>>C;
       ins2.close();
       cout<<C<<endl;
    
       Matrix D;
       D=B*C;
       cout<<D;

 */     

/*


       Matrix L;
       Matrix U;
       B.LU(L,U);
       cout<<L;
       cout<<U;

       Matrix Com;
       Com=L*U;
       cout<<Com;

*/

/*
       Matrix C;
       ifstream ins2(argv[2]);
       ins2>>C;
       ins.close();
    
       Matrix D;
       D=B*C;
      cout<<D<<endl;
*/



//       double det;
//       det=B.det();
//       cout<<B<<endl;
      // cout<<"det value is "<<det<<endl;      
      // Matrix C;
//       C=B.Transpose();
      // C=B.Identity();
/*       cout<<B(1,1)<<endl;
       cout<<B(1,2)<<endl;
       cout<<B(2,1)<<endl;
       cout<<B(2,2)<<endl;

       B.Null(5);
       Matrix A;
       A=B;
*/
  //     ofstream out(argv[2]);
    //   out<<C;
      // out.close();



//       Matrix Q;
  //     Matrix R;

    //   B.QR(Q,R);
     //  cout<<Q;
      // cout<<R;

//       ofstream out(argv[2]);
  //     out<<L;
    //   out.close();
      // cout<<L;


//       ofstream out(argv[2]);
  //     out<<L;
    //   out.close();
      // ofstream out(argv[3]);
      // out<<U;
      // out.close();





/*      
	Matrix x;
	Matrix A;
	Matrix B;
	Matrix C;
	ifstream ins(argv[1]);
	ins>>A;
	ins.close();
	ifstream ins2(argv[2]);
	ins2>>B;
	ins2.close();
        Matrix L;
        Matrix U;
        A.LU(L,U);
        cout<<L;
        cout<<U;

*/

/*	//Testing....
	int err=0;
	ofstream errf("error.txt");
	Matrix L,U;
	
	//Test copy constructor;
	Matrix D(A);
	if(A!=D)
		{
			errf<<"Failure: Copy constructor"<<endl;
			err++;
		}
	//Testing assignment
	C=B;
	if(C!=B)
		{
			errf<<"Failure: Assignment"<<endl;
			err++;
		}	
	//Test Addition
	C=A+B;
	D=B+A;
	if(C!=D)
		{
			errf<<"Failure: Addition"<<endl;
			err++;

		}
	//More.........................




	//Display testing results: YOu can find all the errors in the file
	cout<<"Total Errors: "<<err<<endl;
	errf<<"Total errors: "<<err<<endl;
	errf.close();
*/
	return 0;
}

