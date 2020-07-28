#include <iostream>         /* std::cout */
#include <algorithm>        /* std::min */
#include <assert.h>         /* assert */
#include <unordered_map>    /* unordered_map */
#include <stdint.h>         /* int64_t */
#include <string>           /* to_string */
#include <math.h>           /* pow */

template <typename T>
class Fraction{
	private:

	// Making the class friend to another type to get access to the private members of current class when
	// they want to add up.
    template <typename U> friend class Fraction;

    /*
    void simplify:
    This private function simplifies the fraction ratio so the gcd of the
    numerator and denominator becomes 1 and the ration cannot be simplified anymore.
    Also, the numerator and denominator will be converted into integers if they are
    initially entered as double or float.
    */
	void simplify(T& num,T& den){
         // Taking care of negative numbers
        int sign = 1;
		if ((den<0) ^ (num<0)){
			sign = -1;
		}
		num = std::abs(num);
        den = std::abs(den);
        std::string stNum = std::to_string(num);
        int decimalNum = 0;
        int ind = 0;
        bool found = false;
        for (int i = stNum.size()-1;i>-1;i--){
            if (!found && stNum[i]!='0' && stNum[i]!='.'){
                ind = i;
                found=true;
            }else if (!found && (stNum[i]=='0' || stNum[i]=='.')){
                continue;
            }
            if (found && stNum[i]=='.'){
                decimalNum = ind-i;
            }
        }
        std::string stDen = std::to_string(den);
        int decimalDen = 0;
        found = false;
        for (int i = stDen.size()-1;i>-1;i--){
            if (!found && stDen[i]!='0' && stDen[i]!='.'){
                ind = i;
                found=true;
            }
            else if (!found && (stDen[i]=='0' || stDen[i]=='.')){
                continue;
            }
            if (found && stDen[i]=='.'){
                decimalDen = ind-i;
            }
        }
        int64_t numNoDec = num*pow(10,decimalNum);
        int64_t denNoDec = den*pow(10,decimalDen);

        //Check if there are round off errors. Then compensate and fix!
        if ((abs)(num*pow(10,decimalNum)-numNoDec)>(abs)(num*pow(10,decimalNum)-(numNoDec+1)))
            numNoDec++;
        if ((abs)(den*pow(10,decimalNum)-denNoDec)>(abs)(den*pow(10,decimalNum)-(denNoDec+1)))
            denNoDec++;


        if (decimalNum>decimalDen){
            denNoDec = denNoDec*pow(10,decimalNum-decimalDen);
        }else{
            numNoDec = numNoDec*pow(10,decimalDen-decimalNum);
        }

        for (int64_t i = 2;i<=std::min(numNoDec,denNoDec);i++){
            while (numNoDec%i==0 && denNoDec%i==0){
                numNoDec/=i;
                denNoDec/=i;
            }
        }
        num = sign*numNoDec;
        den = denNoDec;
	}

    // Private class member variables with template type.
	T numerator, denominator;


	public:

    //Constructor of the Fraction class
	Fraction(T num = 1,T den = 1):numerator(num),denominator(den){
		while (denominator==0){
			std::cout<< std::endl;
			std::cout<<"The denominator of the entered Fraction object can't be zero."<<std::endl;
			std::cout<<"Please try a new denominator:"<<std::endl;
			std::cout<<"New denominator:";
			std::cin >> denominator;
            std::cout<<std::endl;
		}
		simplify(numerator,denominator);
	}

    /*
    *   + operator
    *   When a fraction is added to the current Fraction to instantiate or change another Fraction object.
    */
    template<class U>
	Fraction operator +(const Fraction<U>& a) {
		Fraction res;
		res.numerator = numerator*a.denominator + denominator*a.numerator;
		res.denominator = a.denominator*denominator;
		simplify(res.numerator,res.denominator);
		return res;
	}

    /*
	*   + operator
    *   When a number (integer or double or ...) is added to instantiate or change another Fraction object.
    */
    template<class U>
	Fraction operator +(U a) {
		Fraction res;
		res.numerator = numerator + denominator*a;
		res.denominator = denominator;
		simplify(res.numerator,res.denominator);
		return res;
	}

    /*
    *   += operator
    *  When a Fraction object is added to update the current Fraction object.
    */
    template<class U>
	void operator +=(const Fraction<U>& a){
		numerator = numerator*a.denominator + denominator*a.numerator;
		denominator *= a.denominator;
		simplify(numerator,denominator);
	}

    /*
    *   += operator
    *  When a number (integer or double or ...) is added to update the current Fraction object.
    */
    template<class U>
    void operator +=(U a){ // When an integer is added to the other number . When a fraction is added.
		numerator += denominator*a;
		simplify(numerator,denominator);
	}

	/*
    *   - operator
    *   When a Fraction is subtracted to the current Fraction to instantiate or change another Fraction object.
    */
    template<class U>
	Fraction operator -(const Fraction<U>& a) {
		Fraction res(a.numerator,a.denominator);
		res.numerator = numerator*res.denominator - denominator*res.numerator;
		res.denominator *= denominator;
		simplify(res.numerator,res.denominator);
		return res;
	}

    /*
	*   - operator
    *  When a number (integer or double or ...) is subtracted to instantiate or change another Fraction object.
    */
    template<class U>
	Fraction operator -(U a) {
		Fraction res(a,a);
		res.numerator = numerator - denominator*a;
		res.denominator = denominator;
		simplify(res.numerator,res.denominator);
		return res;
	}

    /*
    *   -= operator
    *  When a Fraction object is subtracted to update the current Fraction object.
    */
	template<class U>
	void operator -=(const Fraction<U>& a){
		numerator = numerator*a.denominator - denominator*a.numerator;
		denominator *= a.denominator;
		simplify(numerator,denominator);
	}

    /*
	*   -= operator
    *  When a number (integer or double or ...) is subtracted to update the current Fraction object.
    */
    template<class U>
	void operator -=(U a){
		numerator -= denominator*a;
		simplify(numerator,denominator);
	}

    /*
    *   * operator
    *   When a Fraction object is multiplied with the current Fraction object to instantiate or change another Fraction object.
    */
    template <class U>
    Fraction operator *(const Fraction<U>& a){
		Fraction res(a.numerator,a.denominator);
		res.numerator = a.numerator* numerator;
		res.denominator = a.denominator * denominator;
		simplify(res.numerator,res.denominator);
		return res;
	}

    /*
    *   * operator
    *   When a number (integer or double or ...) is multiplied to instantiate or change another Fraction object.
    */
    template<class U>
	Fraction operator *(U a) {
		Fraction res;
		res.numerator = numerator*a;
		res.denominator = denominator;
		simplify(res.numerator,res.denominator);
		return res;
	}
    /*
    *   *= operator
    *   When a Fraction object is multiplied to update the current Fraction object.
    */
    template <class U>
	void operator *=(const Fraction<U>& a){
		numerator *= a.numerator;
		denominator *= a.denominator;
		simplify(numerator,denominator);
	}

    /*
    *   *= operator
    *   When a number (integer or double or ...) is multiplied to update the current Fraction object.
    */
    template<class U>
	void operator *=(U a){
		numerator = numerator*a;
		simplify(numerator,denominator);
	}

    /*
	*   / operator
    *   When a Fraction object divides the current Fraction object to instantiate or change another Fraction object.
    */
    template<class U>
	Fraction operator /(const Fraction<U>& a) {
		assert(a.numerator!=0);

		Fraction res(a.numerator,a.denominator);
		int temp = res.numerator;
		res.numerator = numerator*res.denominator;
		res.denominator = denominator*temp;
		simplify(res.numerator,res.denominator);
		return res;
	}

    /*
	*   / operator
    *  When a number (integer or double or ...) divides the current Fraction object to instantiate or change another Fraction object.
    */
    template<class U>
	Fraction operator /(U a) {
		assert(a!=0);
		Fraction res(numerator,denominator*a);
		simplify(res.numerator,res.denominator);
		return res;
	}

    /*
	*   / operator
    *   When a Fraction object divides the current Fraction object.
    */
    template<class U>
	void operator /=(const Fraction<U>& a){
		assert(a.numerator!=0);

		int temp = numerator;
		numerator *= a.denominator;
		denominator *= a.numerator;
		simplify(numerator,denominator);
	}

    /*
    *   / operator
    *   When a number (integer or double or ...) divides the current Fraction object.
    */
    template<class U>
	void operator /=(U a){
		assert(a!=0);
		denominator *= a;
		simplify(numerator,denominator);
	}

	//Comparison operator overloading with Fractions.
	template<class U>
	bool operator >(const Fraction<U>& a) const{
		return (this->numerator*a.denominator > this->denominator*a.numerator);
	}

	//Comparison operator overloading with numbers.
	template<class U>
	bool operator >(U a) const{
		return (this->numerator > this->denominator*a);
	}

	//Comparison operator overloading with Fractions.
	template<class U>
	bool operator <(const Fraction<U>& a) const{
		return (this->numerator*a.denominator < this->denominator*a.numerator);
	}

	//Comparison operator overloading with numbers.
	template<class U>
	bool operator <(U a) const{
		return (this->numerator < this->denominator*a);
	}

	//Comparison operator overloading with Fractions.
	template<class U>
	bool operator >=(const Fraction<U>& a) const{
		return (this->numerator*a.denominator >= this->denominator*a.numerator);
	}

	//Comparison operator overloading with numbers.
	template<class U>
	bool operator >=(U a) const{
		return (this->numerator >= this->denominator*a);
	}

	//Comparison operator overloading with Fractions.
	bool operator <=(const Fraction& a) const{
		return (this->numerator*a.denominator <= this->denominator*a.numerator);
	}

	//Comparison operator overloading with numbers.
	template<class U>
	bool operator <=(U a) const{
		return (this->numerator <= this->denominator*a);
	}

	//Comparison operator overloading with Fractions.
	template<class U>
	bool operator ==(const Fraction<U>& a) const{
		return (this->numerator==a.numerator && this->denominator==a.denominator);
	}

	//Comparison operator overloading with numbers.
	template<class U>
	bool operator ==(U a) const{
		return (this->numerator==a && this->denominator==1);
	}

	//Comparison operator overloading with Fractions.
	template<class U>
	bool operator !=(const Fraction<U>& a) const{
		return !((*this)==a);
	}

	//Comparison operator overloading with numbers.
	template<class U>
	bool operator !=(U a) const{
		return !((*this)==a);
	}

	// To print the Fraction object as a ratio. This is similar to pretty function in MATLAB.
	void print() const{
        std::cout<<"Now, the Fraction ratio is:";
        std::cout<<std::endl;
		std::cout<<"    "<<numerator<<std::endl;
		if (denominator==1 || numerator==0){
            std::cout<<std::endl;
            return;
		}
        std::cout<<"    ";
        for (int i = 0;i<std::max(std::to_string((int64_t)numerator).size(),std::to_string((int64_t)denominator).size());i++){
            std::cout<<'-';
        }
        std::cout<<std::endl;
        std::cout<<"    ";
		std::cout<<denominator<<std::endl;
        std::cout<<std::endl;
	}


    /*
    *   To print the Fraction object in decimal form, with recurring decimal in parenthesis.
    *   Examples:
    *       i) 32.1454545... = 32.1(45)
    *       ii) 2/3 = 0.(6)
    */

    std::string display() {
        std::cout<<"The Fraction ratio representation in decimal is:";
        std::cout<<std::endl;
        // zero numerator
        if (!numerator)
            return "    0";
        int sign = (numerator<0 ^ denominator<0) ? -1 : 1;
        std::string str = "    ";
        str += (numerator<0 ^ denominator<0) ? "-" : "";
        numerator = std::abs(numerator);
        denominator = std::abs(denominator);

        std::string stNum = std::to_string(numerator);
        int decimalNum = 0;
        int ind = 0;
        bool found = false;
        for (int i = stNum.size()-1;i>-1;i--){
            if (!found && stNum[i]!='0' && stNum[i]!='.'){
                ind = i;
                found=true;
            }else if (!found && (stNum[i]=='0' || stNum[i]=='.'))
                continue;
            if (found && stNum[i]=='.')
                decimalNum = ind-i;
        }
        std::string stDen = std::to_string(denominator);
        int decimalDen = 0;
        found = false;
        for (int i = stDen.size()-1;i>-1;i--){
            if (!found && stDen[i]!='0' && stDen[i]!='.'){
                ind = i;
                found=true;
            }else if (!found && (stDen[i]=='0' || stDen[i]=='.'))
                continue;
            if (found && stDen[i]=='.')
                decimalDen = ind-i;
        }
        int64_t numNoDec = numerator*pow(10,decimalNum);
        int64_t denNoDec = denominator*pow(10,decimalDen);
        if (std::abs(numerator*pow(10,decimalNum)-numNoDec)>std::abs(numerator*pow(10,decimalNum)-(numNoDec+1)))
            numNoDec++;
        if (std::abs(denominator*pow(10,decimalNum)-denNoDec)>std::abs(denominator*pow(10,decimalNum)-(denNoDec+1)))
            denNoDec++;

        if (decimalNum>decimalDen)
            denNoDec*=pow(10,decimalNum-decimalDen);
        else
            numNoDec*=pow(10,decimalDen-decimalNum);
        for (int64_t i = 2;i<=std::min(numNoDec,denNoDec);i++){
            while (numNoDec%i==0 && denNoDec%i==0){
                numNoDec/=i;
                denNoDec/=i;
            }
        }
        numerator*=sign;
        str += std::to_string(numNoDec / denNoDec);
        int64_t remainder = numNoDec % denNoDec;
        if (!remainder)
            return str;
        str += '.';
        std::unordered_map<int, int> flag;
        int pos = str.size();
        while (remainder && flag.find(remainder) == flag.end()) {
            flag[remainder] = pos++;
            str += '0' + remainder * 10 / denNoDec;
            remainder = remainder * 10 % denNoDec;
        }
        if (remainder) {
            str += ')';
            str.insert(str.begin() + flag[remainder], '(');
        }
        return str;
    }
};


/*
*   The main includes some test cases to check the functionality
*   of the implemented class and its methods.
*/
int main(){
    std::cout<<"test 0::"<<std::endl;
    Fraction<double> a(-3.1,0.200);
    a.print();

    std::cout<<"test 1::"<<std::endl;
    Fraction<double> b(0.6,-0.3);
    Fraction<int> c(63,-25);
    a = b + c;
    a.print();


    std::cout<<"test 2::"<<std::endl;
    Fraction<int> d(-500,700);
    a -= d;
    Fraction<double> j = a;
    j.print();

    std::cout<<"test 3::"<<std::endl;
    a += 0.2;
    a.print();

    std::cout<<"test 4::"<<std::endl;
    Fraction<double> k = a*c;
    k.print();

    std::cout<<"test 5::"<<std::endl;
    Fraction<int> e(200,400);
    a *= e;
    a.print();

    std::cout<<"test 6::"<<std::endl;
    a += 10;
    a.print();

    std::cout<<"test 7::"<<std::endl;
    Fraction<double> f(-20,1300);
    a /= f;
    a.print();

    std::cout<<"test 8::"<<std::endl;
    Fraction<double> g = a*(-2);
    g.print();

    std::cout<<"test 9::"<<std::endl;
    std::cout<<g.display()<<std::endl;

    std::cout<<std::endl;
    std::cout<<"test 10::"<<std::endl;
    int up = 1100;
    int lo = 900;
    int eq = 1050;

    if (g < up){
        std::cout<<"It is less than "<<up<<std::endl;
    }
    else{
        std::cout<<"It is greater"<<up<<std::endl;
    }

    std::cout<<std::endl;
    std::cout<<"test 11::"<<std::endl;

    if (g < lo){
        std::cout<<"It is less than "<<lo<<std::endl;
    }
    else{
        std::cout<<"It is greater "<<lo<<std::endl;
    }

    std::cout<<std::endl;
    std::cout<<"test 12::"<<std::endl;
    // Hard coding the fraction ratios into a new object.
    Fraction<int> h(72319,70);
    if (g == h){
        std::cout<<"The two Fraction objects are equal."<<std::endl;
    }
    else{
        std::cout<<"The two Fraction objects are not equal."<<std::endl;
    }

    return 0;
}
