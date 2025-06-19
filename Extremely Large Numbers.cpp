#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

struct ExtremeNumber{//finish with numbers smaller than 0, division, substraction and comparsion
    vector<int> Number;
    bool negative = false;

    void Set(string NumberString){
        if(NumberString.length()==0){
                cout << "\"Set\" Error: empty string\n";
                return;
        }

        if(NumberString[0]=='-'){
            NumberString.erase(0, 1);
            negative = true;
        }
        else negative = false;

        Number.clear();
        int size = (NumberString.length()-1)/3+1;
        for(int i = 0; i<size; ++i)Number.push_back(0);
        //cout << "Resized: " <<Number[0]+1 << "\n";

        while(NumberString.length()%3>0)NumberString = "0" + NumberString;

        for(int i = Number.size()-1; i>=0 && NumberString !=""; --i){
            string Temp = "";
            for(int i = 0; i<3;++i)Temp += NumberString[i];
            //cout << Temp << " ";
            Number[i] = stoi(Temp);
            //cout << stoi(Temp) << "\n";
            NumberString.erase(0, 3);
        }
    }

    void SetSegment(int Value, int PositionIndex){
        //cout << "Index: " << PositionIndex << "\n";
        while(PositionIndex>Number.size()-1)Number.push_back(0);
        Number[PositionIndex] = Value;
    }

    void AddToSegment(int Value, int PositionIndex){
        //cout << "Index: " << PositionIndex << "\n";
        while(PositionIndex>(int)(Number.size()-1))Number.push_back(0);
        // cout << Number.size();
        Number[PositionIndex] += Value;
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

    void Rearrange(){
        for(int i = 0; i<Number.size(); ++i){
            if(Number[i]>999){
                    AddToSegment(Number[i]/1000, i+1);
                    SetSegment(Number[i]%1000,i);
            }
        }
    }

    ExtremeNumber operator+(const ExtremeNumber& SecondNumber){
        int size = max(SecondNumber.Number.size(), Number.size());
        //for(int i = Number.size(); i<SecondNumber.Number.size(); ++i)Number.push_back(0);
        //cout << "Size: " << Number.size() << "\n";
        for(int i = 0; i<size; ++i){
            if(SecondNumber.Number.size()>i)AddToSegment(SecondNumber.Number[i], i);
        }
        Rearrange();
        return *this;
    }

    ExtremeNumber operator+=(const ExtremeNumber& SecondNumber){
        return *this + SecondNumber;
    }

    ExtremeNumber operator*(const ExtremeNumber& SecondNumber){
        ExtremeNumber temp;
        for(int i = 0; i<Number.size(); ++i){
            for(int j = 0; j<SecondNumber.Number.size(); ++j){
                temp.AddToSegment(Number[i]*SecondNumber.Number[j], j+i);
                if(temp.Number[j+i]>2140000){
                    temp.AddToSegment(temp.Number[j+i]/1000,j+i+1);
                    temp.SetSegment(temp.Number[j+i]%1000, i+j);
                }
            }
        }
        temp.Rearrange();
        return temp;
    }

    ExtremeNumber operator*=(ExtremeNumber& secondNumber){
        *this = *this * secondNumber;
        return *this;
    }

    int Compare(const ExtremeNumber& Number1, const ExtremeNumber& Number2){//1 - first is bigger, 2 - second is bigger, 0 - they are equal
        if(Number1.negative && !Number2.negative)return 2;
        else if(!Number1.negative && Number2.negative) return 1;
        //cout << "Passed minus comparsion\n";
        if(Number1.Number.size()>Number2.Number.size())return 1;
        else if(Number1.Number.size()<Number2.Number.size()) return 2;
        //cout << "Passed size comparsion\n";
        for(int i = Number1.Number.size()-1; i>=0; --i){
            if(Number1.Number[i]>Number2.Number[i]){
                    return 1;
            }
            else if(Number1.Number[i]<Number2.Number[i])return 2;
        }
        //cout << "Passed all comparsions\n";
        return 0;
    }

    //Comparsion operators
    bool operator>(const ExtremeNumber& Number2){
        int Value = Compare(*this, Number2);
        if(Value == 1)return true;
        else return false;
    }

    bool operator>=(const ExtremeNumber& Number2){
        int Value = Compare(*this, Number2);
        if(Value == 2)return false;
        else return true;
    }

    bool operator<(const ExtremeNumber& Number2){
        int Value = Compare(Number2, *this);
        if(Value == 1)return true;
        else return false;
    }

    bool operator<=(const ExtremeNumber& Number2){
        int Value = Compare(Number2, *this);
        if(Value == 2)return false;
        else return true;
    }
    //End of Comparsion operators

    //Comparsions with int
    bool operator<(const int Number2){
        ExtremeNumber N2;
        N2.Set(to_string(Number2));
        return *this < N2;
    }

    bool operator<=(const int Number2){
        ExtremeNumber N2;
        N2.Set(to_string(Number2));
        return *this <= N2;
    }

    bool operator>(const int Number2){
        ExtremeNumber N2;
        N2.Set(to_string(Number2));
        return *this > N2;
    }

    bool operator>=(const int Number2){
        ExtremeNumber N2;
        N2.Set(to_string(Number2));
        return *this >= N2;
    }

};

bool operator>(const int Number1, const ExtremeNumber& Number2) {
    ExtremeNumber N1;
    N1.Set(to_string(Number1)); 
    return N1 > Number2;  
}

bool operator>=(const int Number1, const ExtremeNumber& Number2) {
    ExtremeNumber N1;
    N1.Set(to_string(Number1)); 
    return N1 >= Number2;  
}

bool operator<(const int Number1, const ExtremeNumber& Number2) {
    ExtremeNumber N1;
    N1.Set(to_string(Number1)); 
    return N1 < Number2;  
}

bool operator<=(const int Number1, const ExtremeNumber& Number2) {
    ExtremeNumber N1;
    N1.Set(to_string(Number1)); 
    return N1 <= Number2;  
}


int main(){
    ofstream File;
    File.open("Numbers.txt");
    srand(time(NULL));
    ExtremeNumber Number, Number2, Cont;
    Number.Set("9");
    Number2.Set("1");
    for(int i = 1; i<=3; ++i){
        Cont = Number;
            for(int j = 1; j<=Number2; ++j){
                if(j==10000)break;
                Number=Number2*Number;
                cout << j << " " << Number.Number.size() << "\n";
                File << Number.GetString() << "\n";
            }
        Number2=Number;
        cout << "i = " << i << "\n";
        Number.Print();
        Number = Cont;
    }

    return 0;
}
