#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

struct ExtremeNumber{//finish with numbers smaller than 0, division, substraction and comparsion
    vector<int> Number;

    void Set(string NumberString){
        if(NumberString.length()==0){
                cout << "\"Set\" Error: empty string\n";
                return;
        }

        int IsMinus = 1;
        if(NumberString[0]=='-'){
            NumberString.erase(0, 1);
            IsMinus = -1;
        }

        Number.clear();
        Number.push_back((NumberString.length()-1)/3+1);
        for(int i = 1; i<Number[0]+1; ++i)Number.push_back(0);
        //cout << "Resized: " <<Number[0]+1 << "\n";

        while(NumberString.length()%3>0)NumberString = "0" + NumberString;

        for(int i = Number[0]; i>0 && NumberString !=""; --i){
            string Temp = "";
            for(int i = 0; i<3;++i)Temp += NumberString[i];
            //cout << Temp << " ";
            Number[i] = stoi(Temp)*IsMinus;
            //cout << stoi(Temp) << "\n";
            for(int i = 0; i<3; ++i)NumberString.erase(0, 1);
        }
    }

    void SetSegment(int Value, int PositionIndex){
        //cout << "Index: " << PositionIndex << "\n";
        while(PositionIndex>=Number.size())Number.push_back(0);
        Number[PositionIndex] = Value;
        if(Number[0]<PositionIndex)Number[0] = PositionIndex;
    }

    void Print(bool NewLine=true){
        cout << GetString();
        if(NewLine)cout << "\n";
    }

    string GetString(){
        string StringNumber = "";
        StringNumber += to_string(Number[Number[0]]);
        for(int i = Number[0]-1; i>0;--i){
            int Curr = Number[i];
            if(Curr<0)Curr*=-1;
            if(Curr>=100)StringNumber += to_string(Curr);
            else if(Curr>=10)StringNumber += "0" + to_string(Curr);
            else StringNumber += "00" + to_string(Curr);
        }
        return StringNumber;
    }

    ExtremeNumber operator+(const ExtremeNumber& SecondNumber){
        for(int i = Number.size(); i<SecondNumber.Number.size(); ++i)Number.push_back(0);
        //cout << "Size: " << Number.size() << "\n";
        for(int i = 1; i<Number.size(); ++i){
            if(SecondNumber.Number.size()>i)Number[i]+=SecondNumber.Number[i];
            if(Number[i]>999 && Number.size()-1==i)Number.push_back(0);
            if(Number[i]>999){
                    Number[i+1]+=Number[i]/1000;
                    Number[i]=Number[i]%1000;
            }
            if(Number[i]>0 && Number[0]<i)Number[0] = i;
        }
        return *this;
    }

    ExtremeNumber operator+=(const ExtremeNumber& SecondNumber){
        return *this + SecondNumber;
    }

    ExtremeNumber operator*(const ExtremeNumber& SecondNumber){
        ExtremeNumber Temp;
        for(int i = 1; i<Number.size(); ++i){
            ExtremeNumber Empty;
            for(int j = 1; SecondNumber.Number.size()>j; ++j)Empty.SetSegment(Number[i]*SecondNumber.Number[j], j+i-1);
            //Empty.Print();
            Temp += Empty;
        }
        return Temp;
    }

    ExtremeNumber operator*=(ExtremeNumber& SecondNumber){
        *this = *this * SecondNumber;
        return *this;
    }

    int Compare(const ExtremeNumber& Number1, const ExtremeNumber& Number2){
        if(Number1.Number[Number1.Number[0]]<0 && Number2.Number[Number2.Number[0]]>0)return 2;
        else if(Number1.Number[Number1.Number[0]]>0 && Number2.Number[Number2.Number[0]]<0) return 1;
        //cout << "Passed minus comparsion\n";
        if(Number1.Number.size()>Number2.Number.size())return 1;
        else if(Number1.Number.size()<Number2.Number.size()) return 2;
        //cout << "Passed size comparsion\n";
        for(int i = Number1.Number[0]; i>0; --i){
            if(Number1.Number[i]>Number2.Number[i]){
                    return 1;
            }
            else if(Number1.Number[i]<Number2.Number[i])return 2;
        }
        //cout << "Passed all comparsions\n";
        return 0;
    }

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

    bool operator<=(const int Number2){
        ExtremeNumber N2;
        N2.Set(to_string(Number2));
        int Value = Compare(N2, *this);
        if(Value == 2)return false;
        else return true;
    }

    bool operator>=(const int Number2){
        ExtremeNumber N2;
        N2.Set(to_string(Number2));
        int Value = Compare(*this, N2);
        if(Value == 2)return false;
        else return true;
    }
};


int main(){
    srand(time(NULL));
    ExtremeNumber Number, Number2, Cont;
    Number.Set("9");
    Number2.Set("1");
    for(int i = 1; i<=3; ++i){
        Cont = Number;
            for(int j = 1; Number2>=j; ++j)Number=Number2*Number;
        Number2=Number;
        cout << "i = " << i << "\n";
        Number.Print();
        Number = Cont;
    }

    return 0;
}
