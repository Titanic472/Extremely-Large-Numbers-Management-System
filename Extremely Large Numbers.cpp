#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

struct ExtremeNumber{//finish with substraction
    vector<int> Number;
    bool negative = false;

    void Set(string numberString){
        if(numberString.length()==0){
                cout << "\"Set\" Error: empty string\n";
                return;
        }

        if(numberString[0]=='-'){
            numberString.erase(0, 1);
            negative = true;
        }
        else negative = false;

        Number.clear();
        int size = (numberString.length()-1)/3+1;
        for(int i = 0; i<size; ++i)Number.push_back(0);
        //cout << "Resized: " <<Number[0]+1 << "\n";
        while(numberString.length()%3>0)numberString = "0" + numberString;

        for(int i = Number.size()-1; i>=0 && numberString !=""; --i){
            string Temp = "";
            for(int i = 0; i<3;++i)Temp += numberString[i];
            //cout << Temp << " ";
            Number[i] = stoi(Temp);
            //cout << stoi(Temp) << "\n";
            numberString.erase(0, 3);
        }
    }

    void Print(bool NewLine=true, bool segmentedString = false){
        cout << GetString();
        if(NewLine)cout << "\n";
    }

    string GetString(bool segmentedString = false){
        if(Number.empty()){
            return "0";
        }
        string stringNumber = "";
        if(negative) stringNumber += '-';
        stringNumber += to_string(Number[Number.size()-1]);
        for(int i = Number.size()-2; i>=0;--i){
            if(segmentedString) stringNumber += ' ';
            int Curr = Number[i];
            if(Curr>=100)stringNumber += to_string(Curr);
            else if(Curr>=10)stringNumber += "0" + to_string(Curr);
            else stringNumber += "00" + to_string(Curr);
        }
        return stringNumber;
    }

    private:

    void SetSegment(int value, int positionIndex){
        //cout << "Index: " << positionIndex << "\n";
        while(positionIndex>Number.size()-1)Number.push_back(0);
        Number[positionIndex] = value;
    }

    void AddToSegment(int value, int positionIndex){
        //cout << "Index: " << positionIndex << "\n";
        while(positionIndex>(int)(Number.size()-1))Number.push_back(0);
        // cout << Number.size();
        Number[positionIndex] += value;
    }

    void Rearrange(){
        for(int i = 0; i<Number.size(); ++i){
            if(Number[i]>999){
                    AddToSegment(Number[i]/1000, i+1);
                    SetSegment(Number[i]%1000,i);
            }
        }
        while(Number.size()>1 && Number[Number.size()-1]==0)Number.pop_back();
    }
    
    int Compare(const ExtremeNumber& number1, const ExtremeNumber& number2){//1 - first is bigger, 2 - second is bigger, 0 - they are equal
        if(number1.negative && !number2.negative)return 2;
        else if(!number1.negative && number2.negative) return 1;
        //cout << "Passed minus comparsion\n";
        if(number1.Number.size()>number2.Number.size())return 1;
        else if(number1.Number.size()<number2.Number.size()) return 2;
        //cout << "Passed size comparsion\n";
        for(int i = number1.Number.size()-1; i>=0; --i){
            if(number1.Number[i]>number2.Number[i]){
                    return 1;
            }
            else if(number1.Number[i]<number2.Number[i])return 2;
        }
        //cout << "Passed all comparsions\n";
        return 0;
    }

    ExtremeNumber Add(const ExtremeNumber& secondNumber, bool reverse = false){
        bool secondNumberNegative = secondNumber.negative;
        if(reverse){
            secondNumberNegative = !secondNumberNegative;
        }

        if(negative == secondNumberNegative){
            int size = max(secondNumber.Number.size(), Number.size());
            //for(int i = Number.size(); i<secondNumber.Number.size(); ++i)Number.push_back(0);
            //cout << "Size: " << Number.size() << "\n";
            for(int i = 0; i<size; ++i){
                if(secondNumber.Number.size()>i)AddToSegment(secondNumber.Number[i], i);
            }
            Rearrange();
            return *this;
        }
        else{
            if(Abs(*this)==Abs(secondNumber)){
                Number.clear();
                Number.push_back(0);
                negative = false;
                return *this;
            }
            //No need in comparsion, because in this moment second number absolute value is smaller than value of *this(compared in operator functions)

            for(int i = 0; i < secondNumber.Number.size(); ++i){
                if(Number[i] < secondNumber.Number[i]){
                    --Number[i+1];
                    Number[i] += 1000 - secondNumber.Number[i];
                }
                else Number[i] -= secondNumber.Number[i];
            }
            Rearrange();
            return *this;
        }
    }

    public:

    ExtremeNumber Abs(ExtremeNumber operand){
        operand.negative = false;
        return operand;
    }

    ExtremeNumber operator+(const ExtremeNumber& secondNumber){
        ExtremeNumber temp;
        if(Abs(*this)<Abs(secondNumber)){
            temp = secondNumber;
            return temp.Add(*this);
        }
        else{
            temp = *this;
            return temp.Add(secondNumber);
        }
    }

    ExtremeNumber operator+=(const ExtremeNumber& secondNumber){
        if(Abs(*this)<Abs(secondNumber)){
            ExtremeNumber temp = *this;
            *this = secondNumber;
            return Add(temp);
        }
        else{
            return Add(secondNumber);
        }
    }

    ExtremeNumber operator-(const ExtremeNumber& secondNumber){
        ExtremeNumber temp;
        if(Abs(*this)<Abs(secondNumber)){
            temp = secondNumber;
            temp.negative = !temp.negative;
            return temp.Add(*this);
        }
        else{
            temp = *this;
            return temp.Add(secondNumber, true);
        }
    }

    ExtremeNumber operator-=(const ExtremeNumber& secondNumber){
        if(Abs(*this)<Abs(secondNumber)){
            ExtremeNumber temp = *this;
            *this = secondNumber;
            negative = !negative;
            return Add(temp);
        }
        else{
            return Add(secondNumber, true);
        }
    }

    ExtremeNumber operator*(const ExtremeNumber& secondNumber){
        ExtremeNumber temp;
        for(int i = 0; i<Number.size(); ++i){
            for(int j = 0; j<secondNumber.Number.size(); ++j){
                temp.AddToSegment(Number[i]*secondNumber.Number[j], j+i);
                if(temp.Number[j+i]>2140000){
                    temp.AddToSegment(temp.Number[j+i]/1000,j+i+1);
                    temp.SetSegment(temp.Number[j+i]%1000, i+j);
                }
            }
        }
        temp.Rearrange();
        temp.negative = negative ^ secondNumber.negative;
        return temp;
    }

    ExtremeNumber operator*=(ExtremeNumber& secondNumber){
        *this = *this * secondNumber;
        return *this;
    }

    ExtremeNumber operator*(const int& secondNumber){
        ExtremeNumber N2;
        N2.Set(to_string(secondNumber));
        return *this * N2;
    }

    ExtremeNumber operator*=(int& secondNumber){
        ExtremeNumber N2;
        N2.Set(to_string(secondNumber));
        *this = *this * N2;
        return *this;
    }

    //Comparsion operators
    bool operator>(const ExtremeNumber& number2){
        int value = Compare(*this, number2);
        if(value == 1)return true;
        else return false;
    }

    bool operator>=(const ExtremeNumber& number2){
        int value = Compare(*this, number2);
        if(value == 2)return false;
        else return true;
    }

    bool operator<(const ExtremeNumber& number2){
        int value = Compare(number2, *this);
        if(value == 1)return true;
        else return false;
    }

    bool operator<=(const ExtremeNumber& number2){
        int value = Compare(number2, *this);
        if(value == 2)return false;
        else return true;
    }

    bool operator==(const ExtremeNumber& number2){
        int value = Compare(number2, *this);
        if(value == 0)return true;
        else return false;
    }

    bool operator!=(const ExtremeNumber& number2){
        int value = Compare(number2, *this);
        if(value == 0)return false;
        else return true;
    }
    //End of Comparsion operators

    //Comparsions with int
    bool operator<(const int number2){
        ExtremeNumber N2;
        N2.Set(to_string(number2));
        return *this < N2;
    }

    bool operator<=(const int number2){
        ExtremeNumber N2;
        N2.Set(to_string(number2));
        return *this <= N2;
    }

    bool operator>(const int number2){
        ExtremeNumber N2;
        N2.Set(to_string(number2));
        return *this > N2;
    }

    bool operator>=(const int number2){
        ExtremeNumber N2;
        N2.Set(to_string(number2));
        return *this >= N2;
    }

    bool operator==(const int number2){
        ExtremeNumber N2;
        N2.Set(to_string(number2));
        return *this == N2;
    }

    bool operator!=(const int number2) {
        return !(*this == number2);  
    }
    //End of comparsions with int
};

bool operator>(const int number1, const ExtremeNumber& number2) {
    ExtremeNumber N1;
    N1.Set(to_string(number1)); 
    return N1 > number2;  
}

bool operator>=(const int number1, const ExtremeNumber& number2) {
    ExtremeNumber N1;
    N1.Set(to_string(number1)); 
    return N1 >= number2;  
}

bool operator<(const int number1, const ExtremeNumber& number2) {
    ExtremeNumber N1;
    N1.Set(to_string(number1)); 
    return N1 < number2;  
}

bool operator<=(const int number1, const ExtremeNumber& number2) {
    ExtremeNumber N1;
    N1.Set(to_string(number1)); 
    return N1 <= number2;  
}

bool operator==(const int number1, const ExtremeNumber& number2) {
    ExtremeNumber N1;
    N1.Set(to_string(number1)); 
    return N1 == number2;  
}

bool operator!=(const int number1, const ExtremeNumber& number2) {
    return !(number1 == number2);  
}


int main(){
    ofstream File;
    File.open("Numbers.txt");
    srand(time(NULL));
    ExtremeNumber number, number2, Cont;
    number.Set("9");
    number2.Set("11");
    Cont.Set("-1");
    Cont -= number2;
    number.Print();
    number2.Print();

    int a, b;
    for(int i = 0; i<1000000; ++i){
        a = rand()%1000000000;
        b = rand()%1000000000;
        // cout << "a = " << a << "\nb = " << b << "\n";
        number.Set(to_string(a));
        number2.Set(to_string(b));
        // number.Print();
        // number2.Print();
        Cont = number - number2*(-1);
        if(Cont != a-b*(-1))cout << "Error: " << a << " - " << b << " = " << Cont.GetString() << "\n";
        Cont = number2 - number;
        if(Cont != b-a)cout << "Error: " << b << " - " << a << " = " << Cont.GetString() << "\n";
        Cont = number - number2*(-1);
        if(Cont != a-b*(-1))cout << "Error: " << a << " - " << b*(-1) << " = " << Cont.GetString() << "\n";
        Cont = number2 - number*(-1);
        if(Cont != b-a*(-1))cout << "Error: " << b << " - " << a*(-1) << " = " << Cont.GetString() << "\n";
        Cont = number*(-1) - number2;
        if(Cont != a*(-1)-b)cout << "Error: " << a*(-1) << " - " << b << " = " << Cont.GetString() << "\n";
        Cont = number2*(-1) - number;
        if(Cont != b*(-1)-a)cout << "Error: " << b*(-1) << " - " << a << " = " << Cont.GetString() << "\n";
    }

    //for(int i = 0; i<10000000; ++i)Cont = number - number2;
    Cont.Print();
    /*Number.Set("9");
    number2.Set("1");
    
    for(int i = 1; i<=3; ++i){
        Cont = Number;
            for(int j = 1; j<=number2; ++j){
                if(j==10000)break;
                Number=number2*Number;
                cout << j << " " << Number.Number.size() << "\n";
                File << Number.GetString() << "\n";
            }
        number2=Number;
        cout << "i = " << i << "\n";
        Number.Print();
        Number = Cont;
    }*/

    return 0;
}
