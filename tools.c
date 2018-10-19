// tools.c
//
//
// Bryant W. York
// November 2015

#include "tools.h"

void print_bignum(bignum *n)
{
	int i;

	if (n->signbit == MINUS) printf("- ");
	for (i=n->lastdigit; i>=0; i--)
		printf("%c",'0'+ n->digits[i]);

	printf("\n");
}

void int_to_bignum(int s, bignum *n)
{
	int i;				/* counter */
	int t;				/* int to work with */

	if (s >= 0) n->signbit = PLUS;
	else n->signbit = MINUS;

	for (i=0; i<MAXDIGITS; i++) n->digits[i] = (char) 0;

	n->lastdigit = -1;

	t = abs(s);

	while (t > 0) {
		n->lastdigit ++;
		n->digits[ n->lastdigit ] = (t % 10);
		t = t / 10;
	}

	if (s == 0) n->lastdigit = 0;
}

void initialize_bignum(bignum *n)
{
	int_to_bignum(0,n);
}


int max(int a, int b)
{
	if (a > b) return(a); else return(b);
}

/*	c = a +-/* b;	*/

void add_bignum(bignum *a, bignum *b, bignum *c)
{
	int carry;			/* carry digit */
	int i;				/* counter */

	initialize_bignum(c);

	if (a->signbit == b->signbit) c->signbit = a->signbit;
	else {
		if (a->signbit == MINUS) {
			a->signbit = PLUS;
			subtract_bignum(b,a,c);
			a->signbit = MINUS;
		} else {
                        b->signbit = PLUS;
                        subtract_bignum(a,b,c);
                        b->signbit = MINUS;
		}
		return;
	}

	c->lastdigit = max(a->lastdigit,b->lastdigit)+1;
	carry = 0;

	for (i=0; i<=(c->lastdigit); i++) {
		c->digits[i] = (char) (carry+a->digits[i]+b->digits[i]) % 10;
		carry = (carry + a->digits[i] + b->digits[i]) / 10;
	}

	zero_justify(c);
}


void subtract_bignum(bignum *a, bignum *b, bignum *c)
{
	int borrow;			/* has anything been borrowed? */
	int v;				/* placeholder digit */
	int i;				/* counter */

	initialize_bignum(c);

	if ((a->signbit == MINUS) || (b->signbit == MINUS)) {
                b->signbit = -1 * b->signbit;
                add_bignum(a,b,c);
                b->signbit = -1 * b->signbit;
		return;
        }

	if (compare_bignum(a,b) == PLUS) {
		subtract_bignum(b,a,c);
		c->signbit = MINUS;
		return;
	}

        c->lastdigit = max(a->lastdigit,b->lastdigit);
        borrow = 0;

        for (i=0; i<=(c->lastdigit); i++) {
		v = (a->digits[i] - borrow - b->digits[i]);
		if (a->digits[i] > 0)
			borrow = 0;
		if (v < 0) {
			v = v + 10;
			borrow = 1;
		}

                c->digits[i] = (char) v % 10;
        }

	zero_justify(c);
}

int compare_bignum(bignum *a, bignum *b)
{
	int i;				/* counter */

	if ((a->signbit == MINUS) && (b->signbit == PLUS)) return(PLUS);
	if ((a->signbit == PLUS) && (b->signbit == MINUS)) return(MINUS);

	if (b->lastdigit > a->lastdigit) return (PLUS * a->signbit);
	if (a->lastdigit > b->lastdigit) return (MINUS * a->signbit);

	for (i = a->lastdigit; i>=0; i--) {
		if (a->digits[i] > b->digits[i]) return(MINUS * a->signbit);
		if (b->digits[i] > a->digits[i]) return(PLUS * a->signbit);
	}

	return(0);
}

void zero_justify(bignum *n)
{
	while ((n->lastdigit > 0) && (n->digits[ n->lastdigit ] == 0))
		n->lastdigit --;

        if ((n->lastdigit == 0) && (n->digits[0] == 0))
		n->signbit = PLUS;	/* hack to avoid -0 */
}


void digit_shift(bignum *n, int d)		/* multiply n by 10^d */
{
	int i;				/* counter */

	if ((n->lastdigit == 0) && (n->digits[0] == 0)) return;

	for (i=n->lastdigit; i>=0; i--)
		n->digits[i+d] = n->digits[i];

	for (i=0; i<d; i++) n->digits[i] = 0;

	n->lastdigit = n->lastdigit + d;
}



void multiply_bignum(bignum *a, bignum *b, bignum *c)
{
	bignum row;			/* represent shifted row */
	bignum tmp;			/* placeholder bignum */
	int i,j;			/* counters */

	initialize_bignum(c);

	row = *a;

	for (i=0; i<=b->lastdigit; i++) {
		for (j=1; j<=b->digits[i]; j++) {
			add_bignum(c,&row,&tmp);
			*c = tmp;
		}
		digit_shift(&row,1);
	}

	c->signbit = a->signbit * b->signbit;

	zero_justify(c);
}


void divide_bignum(bignum *a, bignum *b, bignum *c)
{
        bignum row;                     /* represent shifted row */
        bignum tmp;                     /* placeholder bignum */
	int asign, bsign;		/* temporary signs */
        int i,j;                        /* counters */

	initialize_bignum(c);

	c->signbit = a->signbit * b->signbit;

	asign = a->signbit;
	bsign = b->signbit;

	a->signbit = PLUS;
        b->signbit = PLUS;

	initialize_bignum(&row);
	initialize_bignum(&tmp);

	c->lastdigit = a->lastdigit;

	for (i=a->lastdigit; i>=0; i--) {
		digit_shift(&row,1);
		row.digits[0] = a->digits[i];
		c->digits[i] = 0;
		while (compare_bignum(&row,b) != PLUS) {
			c->digits[i] ++;
			subtract_bignum(&row,b,&tmp);
			row = tmp;
		}
	}

	zero_justify(c);

	a->signbit = asign;
	b->signbit = bsign;
}

// end bignum.c package

// Function to return gcd of a and b
int gcd(int a, int b)
{
    if (a == 0)
        return b;
    return gcd(b%a, a);
}

// C function for extended Euclidean Algorithm
int egcd(int a, int b, int *x, int *y)
{
	int x1, y1, xgcd;
	
    // Base Case
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }
 
    xgcd = egcd(b%a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b/a) * x1;
    *y = x1;
 
    return xgcd;
}
void tostring(char str[], int num)
{
	int i, rem, len=0, n;
	n = num;
	while (n != 0)
	{
	  len++;
	  n/= 10;
	}
	for (i=0; i<len; i++)
	{
	  rem = num % 10;
	  num = num / 10;
	  str[len-(i+1)] = rem + '0';
	}
	str[len] = '\0';
}
//Gaussian Elimination
int gaussElim(int nDim, double* pfMatr, double* pfVect, double* pfSolution)
{
  double fMaxElem;
  double fAcc;

  int i , j, k, m;


  for(k=0; k<(nDim-1); k++) // base row of matrix
  {
    // search of line with max element
    fMaxElem = fabs( pfMatr[k*nDim + k] );
    m = k;
    for(i=k+1; i<nDim; i++)
    {
      if(fMaxElem < fabs(pfMatr[i*nDim + k]) )
      {
        fMaxElem = pfMatr[i*nDim + k];
        m = i;
      }
    }
    
    // permutation of base line (index k) and max element line(index m)
    if(m != k)
    {
      for(i=k; i<nDim; i++)
      {
        fAcc               = pfMatr[k*nDim + i];
        pfMatr[k*nDim + i] = pfMatr[m*nDim + i];
        pfMatr[m*nDim + i] = fAcc;
      }
      fAcc = pfVect[k];
      pfVect[k] = pfVect[m];
      pfVect[m] = fAcc;
    }

    if( pfMatr[k*nDim + k] == 0.) return 1; // needs improvement !!!

    // triangulation of matrix with coefficients
    for(j=(k+1); j<nDim; j++) // current row of matrix
    {
      fAcc = - pfMatr[j*nDim + k] / pfMatr[k*nDim + k];
      for(i=k; i<nDim; i++)
      {
        pfMatr[j*nDim + i] = pfMatr[j*nDim + i] + fAcc*pfMatr[k*nDim + i];
      }
      pfVect[j] = pfVect[j] + fAcc*pfVect[k]; // free member recalculation
    }
  }

  for(k=(nDim-1); k>=0; k--)
  {
    pfSolution[k] = pfVect[k];
    for(i=(k+1); i<nDim; i++)
    {
      pfSolution[k] -= (pfMatr[k*nDim + i]*pfSolution[i]);
    }
    pfSolution[k] = pfSolution[k] / pfMatr[k*nDim + k];
  }

  return 0;
}

//
// primes < N by trial division
void prtrdiv(int N)
{
	int  i, j, isqrt, divyes;
	char fn[]="Primes"; 
	char num[10];;
	FILE *f1;
	
	tostring(num,N);
	strcat(fn,num);
	f1 = fopen(fn,"w");
	fprintf(f1,"2");
	i=3;
	divyes=0;
	while (i<N){
		if (divyes==0){fprintf(f1,"\n%d",i);}
		i=i+2;
		isqrt=ceil(sqrt((double)i));
		j = 3;
		divyes=0;
		while (j <= isqrt){
		if ((i%j)==0) divyes=1;
		j=j+2;
		}
	}	
	fclose(f1);	
}

//actual no of primes and expected number by PNT
//printed at every multiple of 1000
void prtrdiv1()
{
	int N,i, j, m, isqrt, divyes, cnt;
	char fn[]="PrimesTable"; 
	int tab[9][3];
	FILE *f1;
//	
	f1 = fopen(fn,"w");
	fprintf(f1,"\nActual No. of Primes and PNT Estimate < %d\n",10000);
	N=1000;
	m = 0;
	while (N < 10000)
	{
	  i = 3;
	  divyes = 0;
	  cnt = 1;
	while (i<N){
		if (divyes==0) cnt++;
		i=i+2;
		isqrt=ceil(sqrt((double)i));
		j = 3;
		divyes=0;
		while (j <= isqrt){
		  if ((i%j)==0) divyes=1;
		  j=j+2;
		}
	}
	tab[m][0] = N;
	tab[m][1] = cnt;
	tab[m][2] = ceil(((double)N)/log((double)N));
	fprintf(f1,"\nN = %d  Actual = %d  pi(N)= %d",tab[m][0], tab[m][1], tab[m][2]);
	N = N + 1000;
	m++;
	}
	fclose(f1);		
}

//list primes by sieve of Eratosthenes
//N < 5000
void esieve(int N)
{
	int i, j, M;
	char fn[]="Primes"; 
	char num[10];;
	int key[5001];
	FILE *f1;
	
	tostring(num,N);
	strcat(fn,num);
	f1 = fopen(fn,"w");
	for (i = 0; i<5001; i++) key[i] = 1;
	M = ceil(sqrt((double)N));
	key[0]=0;
	key[1]=0;
	i = 2;
	while (i < N)
	{ 
		key[i]=1;
		for (j=2*i;  j<N;  j=j+i) key[j] = 0;
		j = i+1;
		while (key[j]==0) j++;
		i = j;
	} 
	fprintf(f1,"\nPrimes less than or equal to %d",N);
	for (i=2; i<N; i++) if (key[i]==1) {fprintf(f1,"\n%d",i);}
	
	fclose(f1);	
}

//list primes by multiplication algorithm
//N < 5000
void primesbymult(int r, int n)
{
	//Lists primes between r and r+2n-1
	//where r is even and n is any positive integer
	//works by eliminating composites
	int c, m, s, i, j;
	int k;
	char fn[]="PrimesByMult"; 
	int key[5001];
	FILE *f1;
	
	f1 = fopen(fn,"w");
	
	c = r+(2*n+1);
	m=(int)(sqrt(c));
	for (k=0; k<=n; k++) key[k] = 0;
	i=3;
	while (i<m)
	{
		s=2*(int)((r+i)/(2*i))+1;
		if (s==1) s=3;
		j=s;
		while (j<= (c/i))
		{
			k=(i*j-r+1)/2;
			key[k]=1;
			j=j+2;
		}
		i=i+2;
	}
	for (k=0; k<=n; k++)
	{
		if (key[k] == 0) fprintf(f1,"\n%d",r+2*k+1);
	}
	fclose(f1);
}

int xmax(float *a, int n)
{
	float mx;
	int i, m;
	mx = a[0];
	for(i=0; i<n; i++){
		if (a[i]>mx) {mx = a[i]; m = i;}
	}
	return m;
}

void swap(unsigned int * a, unsigned int * b)
{
	unsigned int temp;
	temp = *a;
	(*a) = *b;
	(*b) = temp;
}

unsigned int uniform(unsigned int x,unsigned int m)
{
// I realize there may be bias using  linux random mod  m
// I am willing to live with the bias for now
 return (unsigned int) x+(rand() % (m-x));

}

//  generates a random permutation from 1 .. n
//  using Knuth  shuffle
void permute(unsigned int perm[], unsigned int n)
{
	unsigned int i, j;
	for (i = 0; i <= n-2; i++) {
		j = uniform(i,n);
		swap(&perm[i],&perm[j]);
	}
}

unsigned int modpower(unsigned int base, unsigned int power, unsigned int modulus)
{
	unsigned int r, r1, d, a, a1, n, m;
	
	r = 1;
	a = base;
	n = power;
	m = modulus;
	
	while (n > 0)
	{
		d = n&0x1;
		if (d == 1) {r1 = a*r; r = r1%m;}
		else {r1 = r;}
		a1 = a*a;
		a = a1%m;
		n = n>>1;
//		printf("\nd = %u r = %u r(mod m)= %u a= %u a(mod m) = %u",d,r1,r,a1,a);
	}
	
	return r;
}



/* 
 * calculates (a * b) % c taking into account that a * b might overflow 
 */
long long mulmod(long long a, long long b, long long mod)
{
    long long x = 0,y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {    
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}
/* 
 * modular exponentiation
 */
long long modulo(long long base, long long exponent, long long mod)
{
    long long x = 1;
    long long y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}

int miller(long long n, long long b)
{
	// miller test for a strong pseudoprime to the base b
	long long  k, r;
	
	k = n-1;
	r = modulo(b,k,n);
	if (r != 1) return 0; //failure - n fails the test
	while ((0==k%2) && (r==1))
	{
		k=k/2;
		r = modulo(b,k,n);
//		printf("\n k = %lld r = %lld",k,r);
	}
	if ((r==1) || (r==n-1)) return 1; //pass
	if ((r != 1) && (r != n-1)) return 0; //fails
}

/*
 * Miller-Rabin Primality test, iteration signifies the accuracy
 */
int millerrabin(long long p,int iteration)
{
 
    int i;
    long long s;
    if (p < 2)
    {
        return 0;
    }
    if (p != 2 && p % 2==0)
    {
        return 0;
    }
     s = p - 1;
    while (s % 2 == 0)
    {
        s /= 2;
    }
    for (i = 0; i < iteration; i++)
    {
        long long a = rand() % (p - 1) + 1, temp = s;
        long long mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
        {
            return 0;
        }
    }
    return 1;
}
/*
 * compute the continued fraction of a/b to n convergents
 */
 void cfrac(int a, int b, int n, int * q)
 {
	 int i, r, aa, bb;
	 
	 if (b == 1){
		 //a is a rational integer
		 q[0] = a;
		 return;
	 }
	 
	 aa = a;
	 bb = b;
	 q[0] = aa/bb;
	 r = aa%bb;
	 
	 while (i<n)
	 {
		 if (r == 0) break;
//		 printf("\n i = %d aa = %d  bb = %d r = %d q[i-1] = %d",i,aa,bb,r,q[i-1]);
		 aa = bb;
		 bb = r;
		 i++;
		 q[i] = aa/bb;
		 r = aa%bb;
	 }
 }
/* 
compute the convergents of a continued fraction
*/ 
void cfconvg(int n, int * a, double *c)
{
	//computes the n convergents from the n quotients a[n]
	//in the representation of a continued fraction
	//
	double p[100], q[100];
	int k;
	
	if (n > 1)
	{
		p[0] = (double) a[0];
		q[0] = (double) 1;
		c[0] = p[0]/q[0];
	}
	else
	{printf("\nIllegal value for length of continued fraction representat: n = %d\n",n);
	  return;
	  }
	  
	if (n > 2)
	{
		p[1] = ((double) a[0])*((double)a[1])+1.0;
		q[1] = (double)a[1];
		c[1] = p[1]/q[1];
	}
	else
	{printf("\nContinued fraction is a rational integer: c[1] = %lf\n",c[0]);
	  return;
	  }
	
	k = 2;
	while (k<n)
	{
		if (a[k] == 0) break;
//		printf("\n k = %d a[k]= %d p[k-1]=%lf q[k-1]=%lf p[k-2]=%lf q[k-2]=%lf ",k,a[k],p[k-1],q[k-1],p[k-2],q[k-2]);
		p[k] = (((double)a[k])*p[k-1])+p[k-2];
		q[k] = (((double)a[k])*q[k-1])+q[k-2];
		c[k] = p[k]/q[k];
//		printf(" c[k] = %lf",c[k]);
		k++;
	}
}
/* 
compute the continued fraction repr of sqrt(n)
where n is an integer that is not a perfect square
*/ 
int cfsqrt(int n, int * a) 
{
	int P[100], Q[100];
	double sqrtn, x[100];
	int k,k1,ax,stop;
	
	sqrtn = sqrt((double)n);
	
	P[0] = 0;
	Q[0] = 1;
	x[0] = ((((double)P[0])+sqrtn))/(double)Q[0];
	a[0] = (int)x[0];
	
	stop = 0;
	k = 1;
	
	while (k < n)
	{
		k1 = k - 1;
		//printf("\nk-1 = %d P[k-1] = %d Q[k-1] = %d x[k-1] = %lf a[k-1] = %d",k1,P[k1],Q[k1],x[k1],a[k1]);
		//scanf("%d",&ax);
		P[k] = (a[k1]*Q[k1])-P[k1];
		Q[k] = (n - (P[k]*P[k]))/Q[k1];
		//if (Q[k] == 0) {stop = k; break;}
		x[k] = (((double)P[k])+sqrtn)/(double)Q[k];
		a[k] = (int) x[k];	
		if ((k > 1) && (Q[k1] == 1)) {stop = k; break;}
		k++;
	}

	return stop;
}
/* 
computes the prime factors les than maxprime in
the factorization of an integer n.
requires a list of primes (plist) and returns 
a list of prime factors (pflist) terminated by -1
*/ 
void cf_factors(int n, int maxprime, int * plist, int * pflist, int * count) 
{
		int prime, number;
		int i, j;
		
		(*count) = 0;
		
		//printf("\n cf_factors:  n = %d maxprime = %d \n", n, maxprime);
		//i=0;
		//printf("\nplist:\n");
		//while (plist[i] !=0) {printf("\n%d",plist[i]); i++;}
	
		number = n;
		if (number < 0) number = -number;
			
		i = 0;
		j = 0;
		while (number > 1)
		{
			//get next prime
			prime = plist[i];
			//printf("\n prime = %d", prime);
			if (prime <= maxprime)
			{
				while ((number%prime)==0)
				{
					pflist[j] = prime;
					number=number/prime;
					//printf("\n number = %d",number);
					(*count)++;
					j++;
				}
			}else
			{
				pflist[j] = -1;
				number = 1;
			}
			i++;
		}
	//	printf("\npflist\n");
	//	for(i=0; i<j; i++) printf("\n%d",pflist[i]);
}
/* 
factor by continued fractions method:  finds a proper factor
of n, where n is a positive integer
*/ 
void cf_propfact(int num, int maxk, int maxprime, int * plist, int * pflist, int * Qstar_array, int * parray)
{
	int rootn, p, p1, pp, ppp, P, Q;
	int i, n, a, ax, Qstar;
	int m, cnt;
	
	//printf("\ncf_propfact:  n = %d  maxk = %d  maxprime = %d\n",num,maxk,maxprime);
	//i=0;
	//printf("\nplist:\n");
	//while (plist[i] !=0) {printf("\n%d",plist[i]); i++;}
	
	n = num;
	rootn = (int)sqrt((double)n);
	
	P = 0;
	Q = 1;
	pp = 1;
	ppp = 0;
	i = 1;
	
	//printf("\nHere2 rootn = %d  P = %d  Q = %d pp = %d  ppp = %d i = %d\n",rootn,P,Q,pp,ppp,i);
	//scanf("%d",&ax);
	
	while (i <= maxk)
	{
		a = (P+rootn)/Q;
		p = a*pp+ppp;
		p = p - (p/n)*n;
		P = a*Q - P;
		Q = (n-P*P)/Q;
		//printf("\nHere3 rootn = %d a = %d  p = %d P = %d  Q = %d pp = %d  ppp = %d i = %d\n",rootn,a,p,P,Q,pp,ppp,i);
		//scanf("%d",&ax);
		if (i != (i/2*2)) Qstar = -Q;
		else Qstar = Q;
		
		if (p > (n/2)) p1 = p - n;
		else p1 = p;
		// now p1 is smallest residue of p in absolute value
		ppp = pp;
		pp = p;
		//
		cnt = 0;
		cf_factors(Qstar,maxprime,plist,pflist, &cnt);
		printf("\n i = %d p_k(mod n) = %d  Q_k+1^* = %d factorization = ",i,p1,Qstar);
		if (Qstar < 0) printf("-");
		for(m=0; m<cnt; m++) {
			printf("%d",pflist[m]);
			if (m<cnt-1) printf("*");
		}
		printf("\n");
		//scanf("%d",&ax);
		//
		parray[i] = p1;
		Qstar_array[i] = Qstar;
		i++;	
	}
} 