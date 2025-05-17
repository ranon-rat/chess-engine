#include <iostream>

int main(){
    using namespace std;
    for(int i=0;i<10&&i>=0;i--){
        cout<<i<<"\n";
    }
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cout<<i<<" "<<j<<"\n";
            if(j==5){
                 goto for_loop_break;
            }
        }
    }
    for_loop_break:
    return 0;
}